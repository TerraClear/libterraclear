/*
 * Various filters
 * Copyright (C) 2019 TerraClear, Inc.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 * CREATED BY: Koos du Preez - koos@terraclear.com
 * 
*/

#include <iostream>
#include <math.h>

//OPENCV
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>

//OPENCV GPU
#include <opencv2/core/cuda.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudaarithm.hpp>
#include <opencv2/cudawarping.hpp>
#include <opencv2/cudafeatures2d.hpp>


#ifndef VISION_FILTERING_HPP
#define VISION_FILTERING_HPP

namespace terraclear
{
    class vision_filtering 
    {
        public:
            vision_filtering();
            virtual ~vision_filtering();

            //gpu versions of filters and transforms
            static cv::cuda::GpuMat apply_normalization_rgb_gpu(cv::cuda::GpuMat& src_img);
            static cv::cuda::GpuMat apply_gpu_threshold(cv::cuda::GpuMat& src_img, cv::Scalar lowrange, cv::Scalar highrange, bool use_hsv = true);
            static void             apply_gpu_color_contouring(cv::Mat& src_img,  cv::Mat& dst_img, cv::Scalar lowrange, cv::Scalar highrange, bool use_hsv = true);
            static cv::Mat          apply_gpu_warp(cv::Mat src_img, cv::Size dst_size, std::vector<cv::Point> pts_src, std::vector<cv::Point> pts_dst);
            static cv::Mat          apply_gpu_rotate(cv::Mat src_img, float rotation_angle);

            //cpu fitlers and transforms
            static cv::Mat  apply_normalization_rgb(cv::Mat& src_img);
            static cv::Mat  apply_sharpening(cv::Mat& src_img);
            static cv::Mat  apply_brightness(cv::Mat& src_img, float gamma);
            static cv::Mat  apply_rotation(cv::Mat& src_img, cv::Mat& rotation_matrix);
            static cv::Mat  get_rotation_matrix(cv::Mat& src_img, float rotation_angle);
            static void     apply_rotation(cv::Rect& src_rect, cv::Mat& rotation_matrix);

        private:

    };
    
}
#endif /* VISION_FILTERING_HPP */

