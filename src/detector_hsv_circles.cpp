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

#include "detector_hsv_circles.hpp"

namespace terraclear
{
    detector_hsv_circles::detector_hsv_circles(cv::Mat imgsrc) 
        : detector_base(imgsrc)
    {
    }

    detector_hsv_circles::~detector_hsv_circles() 
    {
    }

    std::vector<terraclear::bounding_box> detector_hsv_circles::detect_objects() 
    {
        std::vector<terraclear::bounding_box> ret_vect;
        
        cv::Mat mat_filtered;

        //blur Image a bit first.
        cv::blur(_imgsrc, mat_filtered, cv::Size(10, 10));

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

        cv::medianBlur(mat_filtered, mat_filtered, 5);

        //Vector for all contours.
        std::vector<std::vector<cv::Point>> contours;
        findContours(mat_filtered, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        if (draw_contours)
        {
//            for (auto contour : contours)
//            {
//                cv::Rect rectobj = cv::boundingRect(cv::Mat(contour));
//
//                //get center of mass
//                cv::Mat roi = mat_filtered(rectobj);
//                cv::Moments m = cv::moments(roi, true);
//                cv::Point p(rectobj.x + m.m10/m.m00, rectobj.y + m.m01/m.m00);
//                cv::circle(imgsrc, p, 4, cv::Scalar(0x00, 0xff,0xff), 3, 8);
//            }        
            cv::drawContours(_imgsrc, contours, -1, cv::Scalar(0xff, 0x00, 0x00), 2, 8);
        }

        //find circles in contours..
        std::vector<cv::Vec3f> circles;
        cv::HoughCircles(mat_filtered, circles, cv::HOUGH_GRADIENT, 1,
                     100, // change this value to detect circles with different distances to each other
                     80, 15, //canny edge detector thresholds
                     5, 150 // change the last two parameters (min_radius & max_radius) to detect larger circles
                     );

        //generate inscribed bbox for each circle.
        for(cv::Vec3i c : circles)
        {
            int wh = round( sqrtf(pow(c[2], 2) / 2) ) * 2;
            
            terraclear::bounding_box bbox;
            bbox.x = c[0] - round(wh/2);
            bbox.y =  c[1] - round(wh/2);
            bbox.width = wh;
            bbox.height = wh;
            bbox.class_id = 0;
            bbox.confidence = 1.0f;
            bbox.track_id = 0;
            bbox.frame_count = 0;

            ret_vect.push_back(bbox);

//            cv::Rect rectobj(c[0] - round(wh/2), c[1] - round(wh/2), wh, wh);
//            cv::rectangle(imgsrc, rectobj, cv::Scalar(0x00, 0xff, 0x00), 3, 8, 0 );        
//            cv::circle( imgsrc, cv::Point(c[0], c[1]), c[2], cv::Scalar(0x00, 0xff, 0x00), 3, cv::LINE_AA);
//            cv::circle( imgsrc, cv::Point(c[0], c[1]), 2, cv::Scalar(0x00, 0xff,0xff), 3, cv::LINE_AA);
        }
        
        return ret_vect;
    }
    
}