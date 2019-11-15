/*
 * helper class for calculating velocity of tracked areas in pixels per second 
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
 * CREATED BY: Isabelle Butterfield - isabelle.butterfield@terraclear.com
 * 
*/

#ifndef CAMERA_VELOCITY_CALCULATOR_HPP
#define CAMERA_VELOCITY_CALCULATOR_HPP

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"


#define OPENCV
#define GPU

#include <map>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include "velocity_calculator.hpp"
#include "tracker_optflow.hpp"

namespace tc = terraclear;
namespace terraclear
{
    class camera_velocity_calculator
    {
        public:
            struct velocity_vec
            {
                float x;
                float y;
            };
            
            std::vector<tc::bounding_box> _track_anchors;
            std::map<int, tc::bounding_box> _starting_anchors;
            camera_velocity_calculator(cv::Size dst_size, int track_start_y, int track_end_y, int track_max_travel, int track_xy_size, float time_reset_thresh, int _dist_reset_thresh, int anchor_queue_size, bool two_tracking_rows);
            std::vector<tc::bounding_box> update_tracking (cv::Mat& new_img, uint64_t resting_time);
            
            float get_frame_x_v();
            float get_frame_y_v();
            
            std::vector<tc::bounding_box> get_anchor_tracks();

        private:
            int _track_xy_size;
            int _track_max_travel;
            int _track_start_y;
            std::vector<tc::bounding_box> _track_boxes;
            
            terraclear::velocity_calculator* _calculator_x_v;
            terraclear::velocity_calculator* _calculator_y_v;
            terraclear::velocity_calculator* _calculator_x_a;
            terraclear::velocity_calculator* _calculator_y_a;
            
            tc::tracker_optflow* _tracker_engine;
            bool get_tracked_anchor(std::vector<tc::bounding_box> &bbox_list, tc::bounding_box &anchor);
    };
}

#endif