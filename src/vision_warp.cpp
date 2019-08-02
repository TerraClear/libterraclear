/*
 *  Copyright: TerraClear, Inc - 2019
 *  Author: TerraClear Team
 */

#include <opencv2/core/cuda.hpp>
#include <opencv2/cudaarithm.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudawarping.hpp>
#include <opencv2/calib3d.hpp>

#include "vision_warp.h"

using namespace cv;
using namespace std;

namespace  terraclear
{   
    vision_warp::vision_warp() 
    {
        _sw.start();
    }

    vision_warp::~vision_warp() 
    {
    }
    
    void vision_warp::init_transform()
    {
        //Source & Dest Points
        std::vector<cv::Point2f> points_src;
        points_src.push_back(_source_points.top_left);        
        points_src.push_back(_source_points.top_right);        
        points_src.push_back(_source_points.bottom_right);        
        points_src.push_back(_source_points.bottom_left); 
        
        //calculate target points based on target size..
        roi_transform target_points;
        target_points.top_left.x = 0;
        target_points.top_left.y = 0;
        target_points.top_right.x = _target_size.width;
        target_points.top_right.y = 0;
        target_points.bottom_right.x = _target_size.width;
        target_points.bottom_right.y = _target_size.height;
        target_points.bottom_left.x = 0;
        target_points.bottom_left.y = _target_size.height;
        
        std::vector<cv::Point2f> points_dst;
        points_dst.push_back(target_points.top_left);        
        points_dst.push_back(target_points.top_right);        
        points_dst.push_back(target_points.bottom_right);        
        points_dst.push_back(target_points.bottom_left); 

        //compute transformation matrix.
        _transform_matrix =  cv::findHomography(points_src, points_dst); 

    }
    
    cv::Mat vision_warp::get_transfor_matrix()
    {
        return _transform_matrix;
    }
    
    cv::Mat vision_warp::transform_image(cv::Mat img_src)
    {
        cv::Mat img_result;
        
        //warp original & resize.
        _sw.reset();
        cv::warpPerspective(img_src, img_result, _transform_matrix, _target_size); // do perspective transformation
        _elapsed_us = _sw.get_elapsed_us();
      
        return img_result;
    }
    
    cv::Mat vision_warp::transform_image(cv::Mat img_src, bool flip)
    {
        cv::Mat img_result;
        
        //warp original & resize.
        cv::warpPerspective(img_src, img_result, _transform_matrix, _target_size); // do perspective transformation
            
        _sw.reset();
        if (flip)
        {   
            cv::Mat img_flip;
            cv::flip(img_result, img_flip, 0);
            img_result = img_flip;
        }
        _elapsed_us = _sw.get_elapsed_us();
      
        return img_result;
    }
    
    cv::Mat vision_warp::transform_image_gpu(cv::Mat img_src)
    {
        //copy image to GPU
        cv::cuda::GpuMat gpu_src(img_src);
        cv::cuda::GpuMat gpu_dst;
        
        //warp original & resize.
        _sw.reset();
        cv::cuda::warpPerspective(gpu_src, gpu_dst, _transform_matrix, _target_size); // do perspective transformation
        _elapsed_us = _sw.get_elapsed_us();
        
        //copy GPU image back to regular cv mat
        cv::Mat img_result(gpu_dst);
        
        return img_result;
    }
 
    cv::Mat vision_warp::transform_image_gpu(cv::Mat img_src, bool flip)
    {
        // Initialize final image;
        cv::Mat img_result;
        
        //copy image to GPU
        cv::cuda::GpuMat gpu_src(img_src);
        cv::cuda::GpuMat gpu_dst;
   
        //warp original & resize.
        _sw.reset();
        cv::cuda::warpPerspective(gpu_src, gpu_dst, _transform_matrix, _target_size); // do perspective transformation
        // For flipping negative roll 
        if (flip)
        {
            cv::Mat flip_base;
            cv::cuda::GpuMat flip_gpu;

            flip_gpu.upload(flip_base);
            cv::cuda::flip(gpu_dst, flip_gpu, 0); 
            
            // Copy flipped image back to cpu
            flip_gpu.download(img_result);
            flip_gpu.release();
        }
        else
        {
            // Copy unflipped image back to cpu
            gpu_dst.download(img_result);
            gpu_dst.release();
        }
        _elapsed_us = _sw.get_elapsed_us();

        return img_result;
    } 
    
    cv::Mat_<double> get_dst_corners(std::vector<cv::Point> src, cv::Mat trns)
    {
        // Initialize dst_corners mat
        cv::Mat_<double> dst_corners = (cv::Mat_<double>(4,2) << 0,0,0,0,0,0);  

        for (int i = 0; i < 4; ++i)
        {
            dst_corners.at<double>(i,0) = (double)std::floor((
                                           trns.at<double>(0,0) * src[i].x + 
                                           trns.at<double>(0,1) * src[i].y +
                                           trns.at<double>(0,2))
                                         / 
                                         ( trns.at<double>(2,0) * src[i].x +
                                           trns.at<double>(2,1) * src[i].y +
                                           trns.at<double>(2,2)));

            dst_corners.at<double>(i,1) = (double)std::floor((
                                           trns.at<double>(1,0) * src[i].x  +
                                           trns.at<double>(1,1) * src[i].y  +
                                           trns.at<double>(1,2))
                                         / 
                                         ( trns.at<double>(2,0) * src[i].x  +
                                           trns.at<double>(2,1) * src[i].y  +
                                           trns.at<double>(2,2)));
        }

        return dst_corners;
    }
    
    cv::Mat vision_warp::get_chessboard_corners(cv::Mat img_src, cv::Size board_sz)
    {
        cv::Mat gray_image;
        cv::cvtColor(img_src, gray_image, cv::COLOR_BGR2GRAY);
        
        /* Look for chessboard corners */
        std::vector<Point2d> corners;
        
        bool found = cv::findChessboardCorners(gray_image, board_sz, corners,
                    CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE + CALIB_CB_FAST_CHECK);
        
        /* If we find the chessboard */
        if (found)
        {
            //Get four corners of the board
            std::vector<Point2d> org_corners;
    
            org_corners.push_back(cv::Point((int)(std::floor(corners[board_sz.area() - 1].x)),(int)(std::floor(corners[board_sz.area() - 1].y))));
            org_corners.push_back(cv::Point((int)(std::floor(corners[board_sz.area() - board_sz.width].x)),(int)(std::floor(corners[board_sz.area() - board_sz.width].y))));
            org_corners.push_back(cv::Point((int)(std::floor(corners.begin()->x)),(int)(std::floor(corners.begin()->y))));
            org_corners.push_back(cv::Point((int)(std::floor(corners[board_sz.width-1].x)),(int)(std::floor(corners[board_sz.width-1].y))));
            
            for (int i = 0; i < corners.size(); i++)
            {
                corners[i].x = std::floor(corners[i].x);
                corners[i].y = std::floor(corners[i].y);
            }
            struct myclass {
            bool operator() (cv::Point pt1, cv::Point pt2) { return (pt1.y < pt2.y);}
            } myobject;
            std::sort(corners.begin(), corners.end(), myobject);

            int miny = (corners.begin())->y;
            int maxy = (--corners.end())->y;

            struct myclass2 {
            bool operator() (cv::Point pt1, cv::Point pt2) { return (pt1.x < pt2.x);}
            } myobject1;
            std::sort(corners.begin(), corners.end(), myobject1);

            int minx = (corners.begin())->x;
            int maxx = (--corners.end())->x;

            struct myclass3 {
            bool operator() (cv::Point pt1, cv::Point pt2) { return ((pt1.x < pt2.x) || (pt1.y < pt2.y));}
            } myobject3;
            std::sort(corners.begin(), corners.end(), myobject3);

            // Need to get corners of warped board for finding homography
            std::vector<Point2d> corr_corners;
            // Corners of the rectified chessboard
            corr_corners.push_back(cv::Point(minx,miny));
            corr_corners.push_back(cv::Point(maxx,miny));
            corr_corners.push_back(cv::Point(maxx,maxy));
            corr_corners.push_back(cv::Point(minx,maxy));
            
            cv::Mat H = cv::findHomography(org_corners, corr_corners, cv::RANSAC);
            
            _transform_matrix = H;
            
//            cv::Mat final_img;
//            cv::warpPerspective(img_src, final_img, H, img_src.size());
//
//            cv::Rect rect1(0,0,img_src.cols,y);
//            cv::Mat crop1 = final_img(rect1);
//
//            cv::imwrite("/home/alexwitt/Downloads/resultfull.jpg", crop1);
//
//            cv::Rect rect((img_src.cols/2)-350,0,700,crop1.rows);
//            cv::Mat crop = crop1(rect);
//
//            cv::Mat finalMat;
//            cv::resize(crop, finalMat, cv::Size(700,1200));
            
            return _transform_matrix;   
        }
        else 
        {
            std::string o = "o";
        }
            
    }
    
}
