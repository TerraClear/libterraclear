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

//OPENCV
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

//OPENCV GPU
#include <opencv2/core/cuda.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudaarithm.hpp>
#include <opencv2/imgproc.hpp>

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
            static cv::cuda::GpuMat normalize_rgb_gpu(cv::cuda::GpuMat& src_img);
            
            //cpu fitlers and transforms
            static cv::Mat normalize_rgb(cv::Mat& src_img);
            static cv::Mat sharpen(cv::Mat& src_img);
            static cv::Mat brightness(cv::Mat& src_img, float gamma);
            static cv::Mat rotation(cv::Mat& src_img, float rotation_angle);


        private:

    };
    
}
#endif /* VISION_FILTERING_HPP */

