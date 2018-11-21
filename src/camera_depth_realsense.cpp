/*
 * Specific implementation for RealSense 3D camera class 
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

//only compile for RealSense if required.. 
//i.e you MUST #define TC_USE_REALSENSE or use GCC with -DTC_USE_REALSENSE

#ifdef TC_USE_REALSENSE

#include "camera_depth_realsense.hpp"

namespace terraclear
{
    camera_depth_realsense::camera_depth_realsense() 
    {
        //enable RGB and Depth streams at 30FPS
        //_pipe_config.enable_stream(RS2_STREAM_COLOR,1280,720,RS2_FORMAT_BGR8, 30);
        _pipe_config.enable_stream(RS2_STREAM_COLOR, 1920,1080,RS2_FORMAT_BGR8, 30);
        _pipe_config.enable_stream(RS2_STREAM_DEPTH, 1280,720,RS2_FORMAT_ANY, 30);
        
        _pipe.start(_pipe_config);
    }

    camera_depth_realsense::~camera_depth_realsense() 
    {
        _pipe.stop();
    }
    double camera_depth_realsense::get_depth_internal(uint32_t x, uint32_t y)
    {
        float yscale = _frame_depth->get_height() / _frame_color.rows;
        float xscale = _frame_depth->get_width() / _frame_color.cols;
        
        //get distance, factor in scale between depth & color frames.
        return _frame_depth->get_distance(x * xscale , y * yscale) * 100.00f;
    }
    
    double camera_depth_realsense::get_depth_cm(uint32_t x, uint32_t y)
    {
        //get the mean distance of a X*Y square
        double distance = 0; 
        std::vector<double> distances;

        for (int xw = -avgerage_square_pixels; xw < avgerage_square_pixels; xw++ )
        {
            for (int yh = -avgerage_square_pixels; yh < avgerage_square_pixels; yh++)
            {
                double dist = get_depth_internal(x + xw, y + yh);

                //only use non zeros
                if (dist > 0)
                    distances.push_back(dist);
            }
        }

        //If no readings in square was above zero, output whatever the current reading is.. even if zero..
        if (distances.size() > 0)
        {
            //median
            std::nth_element(distances.begin(), distances.begin() + distances.size()/2, distances.end());
            distance = distances[distances.size()/2];
        }
        else
        {
            distance = get_depth_internal(x, y);       
        }
        
        return distance;
    }
    
    void camera_depth_realsense::update_frames()
    {
        // gathers the rgb image and the depth camera image
        rs2::frameset frames = _pipe.wait_for_frames();

        // align depth to the color stream
        rs2::align align(RS2_STREAM_COLOR);

        rs2::frameset processed = align.process(frames);

        // get the data for the frame and display the image
        _frame_depth = std::make_shared<rs2::depth_frame>(processed.first(RS2_STREAM_DEPTH));

        //grab RGB frame..
        rs2::video_frame cur_color_frame(processed.get_color_frame());

        //convert to cv Mat
        _frame_color = cv::Mat( cur_color_frame.get_height(), cur_color_frame.get_width(), CV_8UC3, (uchar *) cur_color_frame.get_data() );
            
    } 
}

#endif