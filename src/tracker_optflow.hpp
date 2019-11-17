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
            
            // just to avoid extra allocations
            cv::cuda::GpuMat src_mat_gpu;
            cv::cuda::GpuMat dst_mat_gpu, dst_grey_gpu;
            cv::cuda::GpuMat prev_pts_flow_gpu, cur_pts_flow_gpu;
            cv::cuda::GpuMat status_gpu, err_gpu;

            cv::cuda::GpuMat src_grey_gpu;    // used in both functions
            cv::Ptr<cv::cuda::SparsePyrLKOpticalFlow> sync_PyrLKOpticalFlow_gpu;
            cv::cuda::Stream stream;

            std::vector<tc::bounding_box> cur_bbox_vec;
            std::vector<bool> good_bbox_vec_flags;
            
    cv::Mat prev_pts_flow_cpu;
        public:
            tracker_optflow(int _gpu_id = 0, int win_size = 9, int max_level = 3, int iterations = 8000, int _flow_error = -1);
            void update_cur_bbox_vec(std::vector<tc::bounding_box> _cur_bbox_vec);
            void update_tracking_flow(cv::Mat src_mat, std::vector<tc::bounding_box> _cur_bbox_vec);
            std::vector<tc::bounding_box> tracking_flow(cv::Mat dst_mat, bool check_error = true);
    };
}

#endif
