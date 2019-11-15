#ifndef TRACKER_OPTFLOW_HPP
#define TRACKER_OPTFLOW_HPP

#include <memory>
#include <vector>
#include <deque>
#include <algorithm>
#include <unistd.h>
#include <stdio.h>
#include <iostream>

//OPENCV 
#include <opencv2/opencv.hpp>            // C++
#include "opencv2/highgui/highgui_c.h"    // C
#include "opencv2/imgproc/imgproc_c.h"    // C
#include <opencv2/core/utility.hpp>

#include "vision_core.hpp"

namespace tc = terraclear;
namespace terraclear
{
    class tracker_optflow
    {
        private:
            const int _gpu_count;
            const int _gpu_id;
            const int _flow_error;
        public:
            tracker_optflow(int _gpu_id = 0, int win_size = 9, int max_level = 3, int iterations = 8000, int _flow_error = -1);
            void update_cur_bbox_vec(std::vector<tc::bounding_box> _cur_bbox_vec);
            void update_tracking_flow(cv::Mat src_mat, std::vector<tc::bounding_box> _cur_bbox_vec);
            std::vector<tc::bounding_box> tracking_flow(cv::Mat dst_mat, bool check_error = true);
    };
}

#endif
