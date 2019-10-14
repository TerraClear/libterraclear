/*
 * core tools & utils
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

#ifndef VISION_CORE_HPP
#define VISION_CORE_HPP

#include <list>

//OpenCV
#include <opencv2/core.hpp>

//jsoncpp https://linux.tips/programming/how-to-install-and-use-json-cpp-library-on-ubuntu-linux-os
#include <json/json.h>

//locals
#include "filetools.hpp"

//DEFINES FOR ALL CLASSES
#define VISION_CLASS_ROCK "rock"
#define VISION_CLASS_NOT_ROCK "not-rock"
#define VISION_CLASS_BACKGROUND "background"

namespace terraclear
{
    
    //Class ID;s
    enum vision_class_type  : uint16_t
    {
        rock = 0,
        not_rock = 1,
        background= 3
    };
    
    struct bounding_box : cv::Rect
    {
        float       confidence;                 
        vision_class_type  class_id; 
        uint32_t    track_id;        
        uint32_t    frame_count;    
        float       obj_distance;
        float       velocity_x;
        float       velocity_y;
        float       eta_s;
                
        bool        predicted;
        bool        tracked;
        bool        detected;
        bool        annotated;
        
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

    class vision_core 
    {
        public:
            vision_core();
            virtual ~vision_core();

            static void saveBoxesJSON(std::string image_file_name, uint32_t image_width, uint32_t image_height, std::list<bounding_box> bboxes);
            static std::list<bounding_box> readBoxesJSON(std::string json_file_name, uint32_t min_area = 0);
            static std::list<bounding_box> readBoxesTXT(std::string txtfilepath, int image_width, int image_height, uint32_t min_area = 0);
            static void mergeBoxes(std::vector<bounding_box> &object_boxes, uint32_t expand_pixels = 0);
            static cv::Rect getIntersectRect(bounding_box b1, bounding_box b2);   
            
            static vision_class_type classStringToType(std::string in_class_string);
            static std::string classTypeToString(vision_class_type in_class_type);
            
            
        private:

    };

}
#endif /* VISION_CORE_HPP */

