/*
 * detector for finding specific range of colored circles
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

#ifndef DETECTOR_HSV_CIRCLES_HPP
#define DETECTOR_HSV_CIRCLES_HPP

#include "detector_base.hpp"
#include "vision_core.hpp"

namespace terraclear
{
    class detector_hsv_circles : public detector_base
    {
        public:
            
            //default values for various shades of orange.
            cv::Scalar _lowrange = cv::Scalar(0, 140, 80);
            cv::Scalar _highrange = cv::Scalar(20, 255, 255);
            
            bool draw_contours = false;
            
            detector_hsv_circles(cv::Mat imgsrc);
            virtual ~detector_hsv_circles();

            //pure virtual implementation
            std::vector<bounding_box> detect_objects();
            
        private:

    };

}

#endif /* DETECTOR_HSV_CIRCLES_HPP */

