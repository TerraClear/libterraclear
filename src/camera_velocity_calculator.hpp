#ifndef CAMERA_VELOCITY_CALCULATOR_HPP
#define CAMERA_VELOCITY_CALCULATOR_HPP

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"


#define OPENCV
#define TRACK_OPTFLOW
#define GPU
#include <darknet/include/yolo_v2_class.hpp>

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
            struct velocity_vec
            {
                float x;
                float y;
            };
            camera_velocity_calculator(cv::Size dst_size, int track_start_y, int track_end_y, int track_max_travel, int track_offset_y, int track_xy_size, float time_reset_thresh, int _dist_reset_thresh);
            void update_tracking(cv::Mat new_img, bool draw_tracking_info);
            float get_frame_velocity();
            velocity_vec get_frame_velocity_vec();

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
