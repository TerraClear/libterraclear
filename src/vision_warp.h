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
    enum Pattern { CHESSBOARD, CIRCLES_GRID, ASYMMETRIC_CIRCLES_GRID };
    
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
            vision_warp();
            vision_warp(std::string exe_filepath);
            virtual ~vision_warp();

            cv::Mat get_transfor_matrix();
            cv::Mat transform_image(cv::Mat img_src);
            cv::Mat transform_image_crop(cv::Mat img_src);
            cv::Mat transform_image(cv::Mat img_src, bool flip);
            
            cv::Mat transform_image_gpu(cv::Mat img_src);
            cv::Mat transform_image_gpu(cv::Mat img_src, bool flip);
            cv::Mat transform_image_gpu_crop(cv::Mat img_src, cv::Size size);
            
            cv::Mat get_chessboard_transform(cv::Mat img_src, cv::Size chessboard_sz);
            void update_frame(const cv::Mat img_src);
            void undistort_img();
            bool findChessBoard(const cv::Size board_sz);
            void calcChessboardCorners(cv::Size boardSize, float squareSize, terraclear::Pattern patternType);
            void computeC2MC1(const cv::Mat &R1, const cv::Mat &tvec1, const cv::Mat &R2, const cv::Mat &tvec2, cv::Mat &R_1to2, cv::Mat &tvec_1to2);
            cv::Mat init_transform();
            
            uint64_t       _elapsed_us; 
            cv::Mat        _transform_matrix;
            cv::Mat        _undistorted_img;
            cv::Size       _target_size;
            roi_transform  _source_points;
            bool           _board_found;

        private:   
            cv::Mat _img;
            cv::Mat cameraMatrix;
            cv::Mat distCoeffs; 
            std::vector<cv::Point3f> objectPoints;
            std::vector<cv::Point2f> corners; 
            stopwatch _sw;
    };
}

#endif /* VISION_WARP_H */

