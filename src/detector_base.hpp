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


namespace terraclear
{
    struct bounding_box : cv::Rect
    {
        float       confidence;                 
        uint32_t    class_id; 
        std::string class_string;
        uint32_t    track_id;        
        uint32_t    frame_count;    
        float       obj_distance;
        float       velocity_x;
        float       velocity_y;
        float       eta_s;

        uint32_t    vel0_count;
                

      
       bool        predicted;
        bool        tracked;
       bool        detected;
        
        // < operator overload for sorting center box in Y (top to bottom)
        bool operator < (const bounding_box &compare_box) const 
        {
            return (y + height) < (compare_box.y + compare_box.height);
        }

        // > operator overload for sorting.
        bool operator > (const bounding_box &compare_box) const 
        {
            return (y + height) > (compare_box.y + compare_box.height);
        }

        // == operator overload for comparing.
        bool operator == (const bounding_box &compare_box) const 
        {
            // if tracked, ID will be > 0 and id's should be the same
            // if not tracked, ID = 0 and compare actual location & size
            if (track_id > 0) 
                return (track_id == compare_box.track_id);
            else
                return ((x == compare_box.x) && (y == compare_box.y) && 
                        (width == compare_box.width) &&( height == compare_box.height));
        }

        cv::Point get_center()
        {
            cv::Point centerpoint;
            centerpoint.x = (width > 0) ? x + (float) width / 2 : x;
            centerpoint.y = (height > 0) ? y + (float) height / 2 : y;
            
            return centerpoint;
        }
    };

    struct bounding_box_cluster
    {
        uint32_t cluster_id;
        std::vector<bounding_box> clustered_boxes;
    };

    class detector_base 
    {
        public:

            detector_base(cv::Mat imgsrc);
            virtual ~detector_base();

            //pure virtual 
         //   virtual std::vector<bounding_box>   detect_objects() = 0;
            virtual std::vector<std::vector<cv::Vec3d>>   detect_objects()=0 ;
            static void                         saveBoxesJSON(std::string image_file_name, uint32_t image_width, uint32_t image_height, std::list<bounding_box> bboxes);
       
        protected:
            cv::Mat _imgsrc;
            static void mergeBoundingBoxes(std::vector<bounding_box> &object_boxes, uint32_t expand_pixels = 0);
            static cv::Rect getIntersectRect(bounding_box b1, bounding_box b2);   
           
    };

}
#endif /* DETECTOR_BASE_HPP */

