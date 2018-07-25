/*
 * Specific implementation for StereoLabs ZED 3D camera class 
 * Copyright (C) 2017 TerraClear, Inc.
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

#include "camera_depth_zed.hpp"

namespace terraclear
{

    camera_depth_zed::camera_depth_zed(uint32_t camera_index, CameraPosition selected_cam) 
    {
        _selected_camera = selected_cam;
        if(!_videofeed.open(camera_index))
        {
            //TODO: Add custom exception handlers..
            throw std::exception();
        }
        
        //Set Camera Resolution
        _videofeed.set(3, ZED_RESX);
        _videofeed.set(4, ZED_RESY);

    }

    camera_depth_zed::~camera_depth_zed() 
    {
        _videofeed.release();
    }

    void camera_depth_zed::update_frames()
    {
        //get next frame;
        cv::Mat video_frame;
        _videofeed >> video_frame;
        

        //Clip the left or right image if needed..
        cv::Rect rect_roi(0, 0, video_frame.cols, video_frame.rows);
        
        if (_selected_camera == CameraPosition::Left)
            rect_roi = cv::Rect(0, 0, video_frame.cols / 2, video_frame.rows);
        else if (_selected_camera == CameraPosition::Right)
            rect_roi = cv::Rect(video_frame.cols / 2, 0, video_frame.cols, video_frame.rows);
            
        video_frame(rect_roi).copyTo(_frame_color);
        
        
    }

    double camera_depth_zed::get_depth_cm(uint32_t x, uint32_t y)
    {
        return 0;
    }    
    
}