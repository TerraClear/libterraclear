#ifndef CAMERA_VELOCITY_CALCULATOR_HPP
#define CAMERA_VELOCITY_CALCULATOR_HPP

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"


#define OPENCV
#define TRACK_OPTFLOW
#define GPU
#include </home/izzy/ml/darknet/src/yolo_v2_class.hpp>

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
