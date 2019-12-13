/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   vision_filtering.cpp
 * Author: koos
 * 
 * Created on October 7, 2019, 2:33 PM
 */

#include <opencv2/core/types.hpp>
#include <opencv2/core/hal/interface.h>
#include <opencv2/imgproc.hpp>

#include "vision_filtering.hpp"

namespace terraclear
{
    vision_filtering::vision_filtering() 
    {
    }

    vision_filtering::~vision_filtering() 
    {
    }

//START GPU FUNCTIONS    
    cv::cuda::GpuMat vision_filtering::apply_normalization_rgb_gpu(cv::cuda::GpuMat& src_img)
    {
        cv::cuda::GpuMat dst_img;

        //split into individual BGR channels 
        cv::cuda::GpuMat channels_in[3], channels_out[3];
        cv::cuda::split(src_img, channels_in);

        //apply Contrast Limited Adaptive Histogram Equalization
        cv::Ptr<cv::cuda::CLAHE> gpu_clahe = cv::cuda::createCLAHE(2.0, cv::Size(16,16));
        gpu_clahe->apply( channels_in[0], channels_out[0] );
        gpu_clahe->apply( channels_in[1], channels_out[1] );
        gpu_clahe->apply( channels_in[2], channels_out[2] );

        //merge channels back together
        cv::cuda::merge(channels_out, 3,  dst_img);

        //free gpu memory
        channels_in[0].release();
        channels_in[1].release();
        channels_in[2].release();
        channels_out[0].release();
        channels_out[1].release();
        channels_out[2].release();

        return dst_img;
    }


    cv::cuda::GpuMat vision_filtering::apply_gpu_threshold(cv::cuda::GpuMat& src_img, cv::Scalar lowrange, cv::Scalar highrange, bool use_hsv)
    {
        const uchar MAX_UCHAR = 0xff;

        cv::cuda::GpuMat tmp, tmp2;
        cv::cuda::GpuMat mat_parts[3];
        cv::cuda::GpuMat mat_parts_low[3];
        cv::cuda::GpuMat mat_parts_high[3];
        
        //blur Image a bit first using a 5x5 gausian filter
        cv::Ptr<cv::cuda::Filter> filter_blur = cv::cuda::createGaussianFilter(src_img.type(), src_img.type(), cv::Size(5, 5), 15);
        filter_blur->apply(src_img, tmp);
        src_img.release();

        // Transform it to HSV color space
        if(use_hsv)
            cv::cuda::cvtColor(tmp, tmp2, cv::COLOR_BGR2HSV);
        else
            tmp.copyTo(tmp2);
        
        tmp.release();

        //split into 3 channels H, S and V
        cv::cuda::split(tmp2, mat_parts);
        tmp2.release();

        //find range for channel 0
        cv::cuda::threshold(mat_parts[0], mat_parts_low[0], lowrange[0], MAX_UCHAR, cv::THRESH_BINARY);
        cv::cuda::threshold(mat_parts[0], mat_parts_high[0],  highrange[0], MAX_UCHAR, cv::THRESH_BINARY_INV);
        cv::cuda::bitwise_and(mat_parts_high[0], mat_parts_low[0], mat_parts[0]);
        mat_parts_low[0].release();    
        mat_parts_high[0].release();    

        //find range for channel 1
        cv::cuda::threshold(mat_parts[1], mat_parts_low[1], lowrange[1], MAX_UCHAR, cv::THRESH_BINARY);
        cv::cuda::threshold(mat_parts[1], mat_parts_high[1],  highrange[1], MAX_UCHAR, cv::THRESH_BINARY_INV);
        cv::cuda::bitwise_and(mat_parts_high[1], mat_parts_low[1], mat_parts[1]);
        mat_parts_low[1].release();    
        mat_parts_high[1].release();    

        //find range for channel 2
        cv::cuda::threshold(mat_parts[2], mat_parts_low[2], lowrange[2], MAX_UCHAR, cv::THRESH_BINARY);
        cv::cuda::threshold(mat_parts[2], mat_parts_high[2],  highrange[2], MAX_UCHAR, cv::THRESH_BINARY_INV);
        cv::cuda::bitwise_and(mat_parts_high[2], mat_parts_low[2], mat_parts[2]);
        mat_parts_low[2].release();    
        mat_parts_high[2].release();    

        cv::cuda::GpuMat tmp3, tmp4;

        //bitwise AND all channels.
        cv::cuda::bitwise_and(mat_parts[0], mat_parts[1], tmp3);
        mat_parts[0].release();    
        mat_parts[1].release();    

        cv::cuda::bitwise_and(tmp3, mat_parts[2], tmp4);
        mat_parts[2].release();    

        //morphological opening (remove small objects from the foreground)
        int morph_size1 = 2;   
        int morph_size2 = 3;   
        cv::Mat element1 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(morph_size1 * 2 + 1, morph_size1 * 2 + 1), cv::Point(morph_size1, morph_size1));
        cv::Mat element2 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(morph_size2 * 2 + 1, morph_size2 * 2 + 1), cv::Point(morph_size2, morph_size2));
        cv::Ptr<cv::cuda::Filter> filter_erode1 = cv::cuda::createMorphologyFilter(cv::MORPH_ERODE, tmp4.type(), element1);
        cv::Ptr<cv::cuda::Filter> filter_dilate1 = cv::cuda::createMorphologyFilter(cv::MORPH_DILATE, tmp4.type(), element1);
        cv::Ptr<cv::cuda::Filter> filter_erode2 = cv::cuda::createMorphologyFilter(cv::MORPH_ERODE, tmp4.type(), element2);
        cv::Ptr<cv::cuda::Filter> filter_dilate2 = cv::cuda::createMorphologyFilter(cv::MORPH_DILATE, tmp4.type(), element2);

        //erode & dilate to remove tiny spots..
        filter_erode1->apply(tmp4, tmp3);
        filter_dilate1->apply(tmp3, tmp4);
        filter_erode1->apply(tmp4, tmp3);
        filter_dilate2->apply(tmp3, tmp4);

        tmp3.release();

        return tmp4;
    }
    
    
    void vision_filtering::apply_gpu_color_contouring(cv::Mat& src_img,  cv::Mat& dst_img, cv::Scalar lowrange, cv::Scalar highrange, bool use_hsv)
    {
        //default is green..        
        apply_gpu_color_contouring(src_img,  dst_img, lowrange, highrange, use_hsv, cv::Scalar(0x00, 0xff, 0x00), 0.30);
    }

    void vision_filtering::apply_gpu_color_contouring(cv::Mat& src_img,  cv::Mat& dst_img, cv::Scalar lowrange, cv::Scalar highrange, bool use_hsv, cv::Scalar contour_color, float alpha)
    {
            //generate GPU matrices.
            cv::cuda::GpuMat gpu_dst, gpu_src;

            //upload image to GPU
            gpu_src.upload(src_img);

            cv::cuda::cvtColor(gpu_src, gpu_src,  cv::COLOR_BGR2BGRA);
            //color for thresholds.
            cv::cuda::meanShiftFiltering(gpu_src, gpu_dst, 20, 40);
            
            gpu_dst = apply_gpu_threshold(gpu_src, lowrange, highrange, use_hsv);

             //copy GPU image back to regular cv mat
            cv::Mat img_result(gpu_dst);
            gpu_dst.release();
            gpu_src.release();

            //find contours
            std::vector<std::vector<cv::Point>> contours;
            cv::findContours(img_result, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

            if (alpha > 0.75)
            {
                //draw directly without opacity
                for( int i = 0; i< contours.size(); i++ )
                    drawContours(dst_img, contours, i, contour_color, 1, 8);
            }
            else
            {
                //drawing contours on separate image
                cv::Mat drawing = cv::Mat::zeros(dst_img.size(), CV_8UC3);
                for( int i = 0; i< contours.size(); i++ )
                    drawContours(drawing, contours, i, contour_color, 1, 8);

                //alpha blend contour temp image with opacity onto dst image.
                cv::addWeighted(drawing, alpha, dst_img, 1.0, 0.5, dst_img);
            }
    }
     

    cv::Mat vision_filtering::apply_gpu_warp(cv::Mat src_img, cv::Size dst_size, std::vector<cv::Point> pts_src, std::vector<cv::Point> pts_dst)
    {
        cv::cuda::GpuMat gpu_src(src_img);
        cv::cuda::GpuMat gpu_dst;

        // Calculate Homography
        cv::Mat h_mat = cv::findHomography(pts_src, pts_dst);

        //GPU warp original & resize.
        cv::cuda::warpPerspective(gpu_src, gpu_dst, h_mat, dst_size); // do perspective transformation

        //copy GPU image back to regular cv mat
        cv::Mat ret_img(gpu_dst);    

        gpu_src.release();
        gpu_dst.release();

        return ret_img;
    }

    cv::Mat vision_filtering::apply_gpu_rotate(cv::Mat src_img, float rotation_angle)
    {
        cv::cuda::GpuMat gpu_src(src_img);
        cv::cuda::GpuMat gpu_dst;

        cv::Point img_center(src_img.cols / 2, src_img.rows /2);
        cv::Size dst_size(src_img.cols, src_img.rows);

        // Calculate rotation matrix 
        cv::Mat r_mat = cv::getRotationMatrix2D(img_center, rotation_angle, 1.0);

        //GPU rotate.
        cv::cuda::warpAffine(gpu_src, gpu_dst, r_mat, dst_size); // do perspective transformation

        //copy GPU image back to regular cv mat
        cv::Mat ret_img(gpu_dst);    

        gpu_src.release();
        gpu_dst.release();

        return ret_img;
    }
    
//  END GPU FUNCTIONS    
   
    cv::Mat vision_filtering::apply_normalization_rgb(cv::Mat& src_img)
    {
        cv::Mat dst_img;

        //split into individual BGR channels 
        cv::Mat channels_in[3], channels_out[3];
        cv::split(src_img, channels_in);

        //apply Contrast Limited Adaptive Histogram Equalization
        cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(2.0, cv::Size(16,16));
        clahe->apply( channels_in[0], channels_out[0] );
        clahe->apply( channels_in[1], channels_out[1] );
        clahe->apply( channels_in[2], channels_out[2] );

        //merge channels back together
        cv::merge(channels_out, 3,  dst_img);

        return dst_img;
    }

    cv::Mat vision_filtering::apply_sharpening(cv::Mat& src_img)
    {
        cv::Mat dst_img;

        //3x3 sharpening filter..
        float filter_conent[] = { 0,-1, 0, 
                                 -1, 5,-1, 
                                  0,-1, 0};

        //construct filter
        cv::Mat kernel(3,3,CV_32F, filter_conent);

        //Apply Filter
        cv::filter2D(src_img, dst_img, -1, kernel);

        return dst_img;
    }

    cv::Mat vision_filtering::apply_brightness(cv::Mat& src_img, float gamma)
    {
        cv::Mat dst_img;

        float inv_gamma  = 1.0f / gamma;

        //construct lookup table for brightness adjustments
        cv::Mat lookUpTable(1, 256, 0x00);
        uchar* p = lookUpTable.ptr();
        for ( int i = 0; i < 255; ++i)
        {
            //calc and constrain lookup table within uchar min / max
            p[i] = cv::saturate_cast<uchar>(pow(i / 255.0, inv_gamma) * 255.0);       
        }

        //apply even Lookup Table Transform to all channels.
        cv::LUT(src_img, lookUpTable, dst_img);

        return dst_img;
    }

    cv::Mat vision_filtering::apply_rotation(cv::Mat& src_img, cv::Mat& rotation_matrix)
    {
        cv::Mat dst_img;
        cv::Size dst_size(src_img.cols, src_img.rows);

        //Rotate.
        cv::warpAffine(src_img, dst_img, rotation_matrix, dst_size); 

        return dst_img;
    }    
    
    
    cv::Mat vision_filtering::get_rotation_matrix(cv::Mat& src_img, float rotation_angle)
    {
        cv::Point img_center(src_img.cols / 2, src_img.rows /2);

        // Calculate rotation matrix 
        cv::Mat r_mat = cv::getRotationMatrix2D(img_center, rotation_angle, 1.0);

        return r_mat;
    }
    
    void vision_filtering::apply_rotation(cv::Rect& src_rect, cv::Mat& rotation_matrix)
    { 
        //top left, top right, bottom right, bottom left
        std::vector<cv::Point3d> src_points;
        src_points.push_back(cv::Point3d(src_rect.x, src_rect.y, 1));
        src_points.push_back(cv::Point3d(src_rect.x + src_rect.width, src_rect.y, 1));
        src_points.push_back(cv::Point3d(src_rect.x + src_rect.width, src_rect.y + src_rect.height, 1));
        src_points.push_back(cv::Point3d(src_rect.x, src_rect.y + src_rect.height, 1));

        cv::Point br(0,0);
        cv::Point tl(1000000,1000000);
        
        std::vector<cv::Point> dst_points;
        for (auto src_point : src_points)
        {
           cv::Mat dst_mat  = rotation_matrix * cv::Mat(src_point);
           tl.x = std::min<double>(tl.x, dst_mat.at<double>(0,0));
           tl.y = std::min<double>(tl.y, dst_mat.at<double>(0,1));
           br.x = std::max<double>(br.x, dst_mat.at<double>(0,0));
           br.y = std::max<double>(br.y, dst_mat.at<double>(0,1));
        }

        src_rect.x = tl.x;
        src_rect.y = tl.y;
        src_rect.width = br.x - tl.x;
        src_rect.height = br.y - tl.y;
    }
    
}
