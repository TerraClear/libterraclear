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
 * CREATED BY: Isabelle Butterfield - isabelle.butterdield@terraclear.com
 * 
*/


#ifndef CAMERA_VELOCITY_CALCULATOR_HPP
#define CAMERA_VELOCITY_CALCULATOR_HPP

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"

#define OPENCV
#define TRACK_OPTFLOW
#define GPU
#include "darknet/include/yolo_v2_class.hpp"

#include <map>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include "velocity_calculator.hpp"

namespace terraclear
{
    class camera_velocity_calculator
    {
        public:
            camera_velocity_calculator(cv::Size dst_size, int track_start_y, int track_end_y, int track_max_travel, int track_offset_y, int track_xy_size);
            cv::Mat update_tracking(cv::Mat new_img);
            float get_frame_velocity();

        private:
            int _track_xy_size;
            int _track_max_travel;
            std::vector<bbox_t> _track_anchors;
            std::vector<bbox_t> _track_boxes;
            terraclear::velocity_calculator* _calculator;
            Tracker_optflow* _tracker_engine;
            bool get_tracked_anchor(std::vector<bbox_t> &bbox_list, bbox_t &anchor);
    };
}

#endif
