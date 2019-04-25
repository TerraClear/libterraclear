/*
 * basic motion detector
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

#include "detector_motion.hpp"

namespace terraclear
{
    detector_motion::detector_motion(cv::Mat imgsrc) 
        : detector_base(imgsrc) 
    {
    }

    detector_motion::~detector_motion() 
    {
    }
    
     void detector_motion::set_motion_threshold(double motion_threshold)
     {
         _motion_threshold = motion_threshold;
     }
    
    std::vector<bounding_box> detector_motion::detect_objects()
    {
        
        std::vector<bounding_box> detections;

        //mats for new frame to compare to previous & working frame
        cv::Mat imgsrc_new,imgsrc_worker;
        
        //copy and convert to grayscale
        //_imgsrc.copyTo(imgsrc_new);
        cv::cvtColor(_imgsrc, imgsrc_new, CV_BGR2GRAY);

        //only perform motion detection on frame 2 onwards.
        if (_detect_count > 0)
        {
            //get abs didd and threshold differences between new and old
            cv::absdiff(imgsrc_new, _imgsrc_old, imgsrc_worker);
            cv::threshold(imgsrc_worker, imgsrc_worker, 20, 255, cv::THRESH_BINARY);
            
            //Create element for morpj\h
            int morph_size = 20;  
            cv::Mat element = getStructuringElement(cv::MORPH_RECT,
                   cv::Size(2 * morph_size + 1, 2 * morph_size + 1),
                   cv::Point(morph_size, morph_size) );

            //erode/dilate morph to make distinct blobs.
             cv::dilate(imgsrc_worker, imgsrc_worker, element);
             cv::erode(imgsrc_worker, imgsrc_worker, element);          

            //find all outer contours around blobs.
            std::vector<std::vector<cv::Point>> contours;
            cv::findContours(imgsrc_worker, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);   
            
            int trackid = 0; //unique ID per bounding box..
            for(auto contour : contours)
            {
                trackid++;

                //calculate the area of contour.
                double area =  cv::contourArea(contour);

                //if contour area (i.e. area changed between frames) is big enough, note it..
                if (area > _motion_threshold)
                {
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

                        detections.push_back(bbox);
                }
            }            
            
        }

        //retain current img frame
        _imgsrc_old = imgsrc_new;
        _detect_count++;
        
        //merge overlapping boxes.
        mergeBoundingBoxes(detections, 20);

        return detections;
    }
}