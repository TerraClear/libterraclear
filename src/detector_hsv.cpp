/*
 * basic hsv color detector
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

#include "detector_hsv.hpp"

namespace terraclear
{
    
    detector_hsv::detector_hsv(cv::Mat imgsrc) 
        : detector_base(imgsrc) 
    {
    }

    detector_hsv::~detector_hsv() 
    {
    }

    std::vector<terraclear::bounding_box> detector_hsv::detect_objects() 
    {
        //ret vector
        std::vector<bounding_box> ret_vect;

        cv::Mat mat_filtered;

        //blur Image a bit first.
        cv::blur(_imgsrc, mat_filtered, cv::Size(5,5));

        /// Transform it to HSV color space
        cv::cvtColor(mat_filtered, mat_filtered, cv::COLOR_BGR2HSV);

        //find all objects in range
        cv::inRange(mat_filtered, _lowrange, _highrange, mat_filtered);


        //morphological opening (remove small objects from the foreground)
        cv::erode(mat_filtered, mat_filtered, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2, 2)) );
        cv::dilate( mat_filtered, mat_filtered, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3)) ); 

         //morphological closing (fill small holes in the foreground)
        cv::dilate( mat_filtered, mat_filtered, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3)) ); 
        cv::erode(mat_filtered, mat_filtered, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2, 2)) );

        //Vector for all contours.
        std::vector<std::vector<cv::Point>> contours;
        findContours(mat_filtered, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        //create bounding boxes from contours
        int trackid = 0; //unique ID per bounding box..
        for (auto contour : contours)
        {
            trackid++;
            
            cv::Rect rect = cv::boundingRect(cv::Mat(contour));
            bounding_box bbox;
            bbox.x = rect.x;
            bbox.y = rect.y;
            bbox.width = rect.width;
            bbox.height = rect.height;
            bbox.class_id = 0;
            bbox.confidence = 1.0f;
            bbox.track_id = trackid;
            bbox.frame_count = 0;

            ret_vect.push_back(bbox);
        }

        //cluster into larger boxes
       mergeBoundingBoxes(ret_vect, 60);
        
        //TODO: iterate through until no overlaps..
        //twice seems to do the trick..
        mergeBoundingBoxes(ret_vect, 60);

        return ret_vect;
    }


}
