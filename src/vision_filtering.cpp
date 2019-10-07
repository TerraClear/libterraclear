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

#include "vision_filtering.hpp"

namespace terraclear
{
    vision_filtering::vision_filtering() 
    {
    }

    vision_filtering::~vision_filtering() 
    {
    }

    cv::cuda::GpuMat vision_filtering::normalize_rgb_gpu(cv::cuda::GpuMat& src_img)
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

    cv::Mat vision_filtering::normalize_rgb(cv::Mat& src_img)
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

    cv::Mat vision_filtering::sharpen(cv::Mat& src_img)
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

    cv::Mat vision_filtering::brightness(cv::Mat& src_img, float gamma)
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

    cv::Mat vision_filtering::rotation(cv::Mat& src_img, float rotation_angle)
    {
        cv::Mat dst_img;

        cv::Point img_center(src_img.cols / 2, src_img.rows /2);
        cv::Size dst_size(src_img.cols, src_img.rows);

        // Calculate rotation matrix 
        cv::Mat r_mat = cv::getRotationMatrix2D(img_center, rotation_angle, 1.0);

        //Rotate.
        cv::warpAffine(src_img, dst_img, r_mat, dst_size); 

        return dst_img;
    }    
}
