/*
 *  Copyright: TerraClear, Inc - 2019
 *  Author: TerraClear Team
 */
#include <iostream>

#include <opencv2/core/cuda.hpp>
#include <opencv2/cudaarithm.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudawarping.hpp>
#include <opencv2/calib3d.hpp>

#include "vision_warp.hpp"
#include "filetools.hpp"

using namespace cv;
using namespace std;

namespace  terraclear
{  
    vision_warp::~vision_warp() 
    {
    }

    cv::Mat* vision_warp::get_transform_matrix()
    {
        return _transform_matrix;
    }
    
    cv::Mat vision_warp::transform_image(cv::Mat img_src)
    {
        cv::Mat img_result;
        //warp original & resize.
        _sw.reset();
        cv::warpPerspective(img_src, img_result, *_transform_matrix, img_src.size()); // do perspective transformation
        _elapsed_us = _sw.get_elapsed_us();
      
        return img_result;
    }
    
    cv::Mat vision_warp::transform_image(cv::Mat img_src, bool flip)
    {
        cv::Mat img_result;
        
        //warp original & resize.
        cv::warpPerspective(img_src, img_result, *_transform_matrix, _target_size); // do perspective transformation
            
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
        cv::cuda::warpPerspective(gpu_src, gpu_dst, *_transform_matrix, _target_size); // do perspective transformation
        _elapsed_us = _sw.get_elapsed_us();
        
        //copy GPU image back to regular cv mat
        cv::Mat img_result(gpu_dst);
        
        return img_result;
    }
    
    cv::Mat vision_warp::transform_image_crop(cv::Mat img_src)
    {
        cv::Mat img_result;
        //warp original & resize.
        _sw.reset();
        cv::warpPerspective(img_src, img_result, *_transform_matrix, img_src.size()); // do perspective transformation
        
        cv::Rect myROI((img_result.cols/2) - ((int)_target_size.width/2), 0, _target_size.width, img_result.rows);
        cv::Mat crop = img_result(myROI);

        cv::Mat resize;
        cv::resize(crop, resize,_target_size, 0, 0, cv::INTER_CUBIC);
        _elapsed_us = _sw.get_elapsed_us();
      
        return resize;
    }
    
    cv::Mat vision_warp::transform_image_gpu_crop(cv::Mat img_src)
    {
        //copy image to GPU
        cv::cuda::GpuMat gpu_src(img_src);
        cv::cuda::GpuMat gpu_dst;
        
        //warp original & resize.
        _sw.reset();
        cv::cuda::warpPerspective(gpu_src, gpu_dst, *_transform_matrix, img_src.size(), cv::RANSAC); // do perspective transformation
        cv::Mat img_result(gpu_dst);
        
        cv::Rect myROI((img_result.cols /2) - _target_size.width/2, 0, _target_size.width, img_result.rows);
        cv::Mat crop = img_result(myROI);
        cv::Mat resize;
        cv::resize(crop,resize,cv::Size(700,1200),0,0,cv::INTER_CUBIC);
        _elapsed_us = _sw.get_elapsed_us();
        
        return resize;
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
        cv::cuda::warpPerspective(gpu_src, gpu_dst, *_transform_matrix, _target_size); // do perspective transformation
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
    
}
