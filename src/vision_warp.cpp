/*
 *  Copyright: TerraClear, Inc - 2019
 *  Author: TerraClear Team
 */

#include <opencv2/core/cuda.hpp>
#include <opencv2/cudawarping.hpp>
#include <opencv2/cudaarithm.hpp>

#include "vision_warp.h"

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
    
}
