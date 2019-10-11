/*
 *  Copyright: TerraClear, Inc - 2019
 *  Author: TerraClear Team
 */

#ifndef VISION_WARP_H
#define VISION_WARP_H

#include <vector>
#include <opencv2/imgproc.hpp>
#include "opencv2/calib3d.hpp"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>

#include "stopwatch.hpp"

#define OPENCV
#define GPU

using namespace std;

namespace  terraclear
{
    
    class vision_warp 
    {
        protected:
            struct  roi_transform
            {
                cv::Point2f top_left;
                cv::Point2f top_right;
                cv::Point2f bottom_right;
                cv::Point2f bottom_left;
            };

        public:
        
            virtual ~vision_warp();

            cv::Mat* get_transform_matrix();
            cv::Mat transform_image(cv::Mat img_src);
            cv::Mat transform_image_crop(cv::Mat img_src);
            cv::Mat transform_image(cv::Mat img_src, bool flip);
            
            cv::Mat transform_image_gpu(cv::Mat img_src);
            cv::Mat transform_image_gpu(cv::Mat img_src, bool flip);
            cv::Mat transform_image_gpu_crop(cv::Mat img_src);
            
            uint64_t _elapsed_us; 
            cv::Mat* _transform_matrix;
            cv::Size _target_size;
            float    _block_size;
            roi_transform  _source_points;

        private:   
            cv::Mat _img;
            stopwatch _sw;
    };
}

#endif /* VISION_WARP_H */

