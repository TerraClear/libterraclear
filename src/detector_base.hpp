/*
 * base class for detectors
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


#ifndef DETECTOR_BASE_HPP
#define DETECTOR_BASE_HPP

#include <vector>
#include <map>

//jsoncpp https://linux.tips/programming/how-to-install-and-use-json-cpp-library-on-ubuntu-linux-os
#ifdef __linux__
    #include <jsoncpp/json/json.h>
#else
    #include <json/json.h>
#endif

//OPENCV
#include <opencv2/opencv.hpp>

//internal includes
#include "filetools.hpp"
#include "vision_core.hpp"

namespace terraclear
{

    class detector_base 
    {
        public:

            detector_base(cv::Mat imgsrc);
            virtual ~detector_base();

            //pure virtual 
            virtual std::vector<bounding_box>   detect_objects() = 0;
       
        protected:
            cv::Mat _imgsrc;
           
    };

}
#endif /* DETECTOR_BASE_HPP */

