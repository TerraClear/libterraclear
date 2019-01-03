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
//i.e you MUST #define TC_USE_REALSENSE or use g++ with -DTC_USE_REALSENSE

#ifdef TC_USE_REALSENSE

#include "camera_depth_realsense.hpp"

namespace terraclear
{
    camera_depth_realsense::camera_depth_realsense() 
    {
        //enable RGB and Depth streams at 30FPS
        _pipe_config.enable_stream(RS2_STREAM_COLOR, 1920,1080,RS2_FORMAT_BGR8, 30);
        _pipe_config.enable_stream(RS2_STREAM_DEPTH, 1280,720,RS2_FORMAT_Z16, 30);

        _pipe.start(_pipe_config);
        
        //_depth_frame = cv::Mat(720, 1280, CV_8UC3);
        
    }

    camera_depth_realsense::~camera_depth_realsense() 
    {
        _pipe.stop();
    }
    double camera_depth_realsense::get_depth_internal(uint32_t x, uint32_t y)
    {
        float yscale = _rls_frame_depth->get_height() / _frame_color.rows;
        float xscale = _rls_frame_depth->get_width() / _frame_color.cols;
        
        //get distance, factor in scale between depth & color frames.
        return _rls_frame_depth->get_distance(x * xscale , y * yscale) * 100.00f;
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

        //grab RGB frame..
        rs2::video_frame cur_color_frame(processed.get_color_frame());
    
        //convert RGB frame to cv Mat
        _frame_color = cv::Mat( cur_color_frame.get_height(), cur_color_frame.get_width(), CV_8UC3, (uchar*) cur_color_frame.get_data() );

        //get depth frame..
        rs2::frame dframe = processed.get_depth_frame();   

        // make shared ptr to frame (i.e. retain ref)
        _rls_frame_depth = std::make_shared<rs2::depth_frame>(dframe);

//        //setup and apply decimation filter with default options
//        rs2::decimation_filter decimation_filter;
//        dframe = dframe.apply_filter(decimation_filter);
//        
//        //setup and apply spacial filter with default options
//        rs2::spatial_filter spacial_filter;
//        dframe = dframe.apply_filter(spacial_filter);
//        
//        //setup and apply temporal filter with default options..
//        rs2::temporal_filter temporal_filter;
//        dframe = dframe.apply_filter(temporal_filter);
//        
//        //setup and apply hole filling filter
//        rs2::hole_filling_filter hole_filter;
//        hole_filter.set_option(RS2_OPTION_HOLES_FILL, 2.0f); //0 0 = Fill From Left, 1 = Nearest, 2 = Furthest.
//        dframe = dframe.apply_filter(hole_filter);
        
        //generate colorized depth image and apply filter
        rs2::colorizer color_map;
        color_map.set_option(RS2_OPTION_HISTOGRAM_EQUALIZATION_ENABLED, 1.0f);
        color_map.set_option(RS2_OPTION_COLOR_SCHEME, 2.0f); // 0 = Jet, 1=  classic, 2 = White to Black        
        
        //setup and apply colorizer filter
        color_map.colorize(dframe);
        dframe = dframe.apply_filter(color_map);
        
        // Query depth frame size (width and height)
        const int depth_w = dframe.as<rs2::video_frame>().get_width();
        const int depth_h = dframe.as<rs2::video_frame>().get_height();
        
        // convert to opencv mat & copy to shared        
       cv::Mat tmp_mat(depth_h, depth_w, CV_8UC3, (uchar*)dframe.get_data(), cv::Mat::AUTO_STEP);
       tmp_mat.copyTo(_depth_frame);


    } 
    


}

#endif