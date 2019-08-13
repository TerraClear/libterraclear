/* 
 * File:   cameracalibration.h
 * Author: Alex Witt
 *
 * Created on August 8, 2019, 4:32 PM
 */

#ifndef CAMERAWARP_H
#define CAMERAWARP_H

#include <opencv2/opencv.hpp>

namespace terraclear
{
    enum Pattern { CHESSBOARD, CIRCLES_GRID, ASYMMETRIC_CIRCLES_GRID };
    
    class camerawarp 
    {
        public:
            cv::Mat _undistorted_img;
            bool   _board_found;
            cv::Mat _H;
            camerawarp();
            void get_camera_paramters(std::string filepath);
            void update_frame(const cv::Mat img_src);
            void undistort_img();
            bool findChessBoard(const cv::Size board_sz);
            virtual ~camerawarp();
            void calcChessboardCorners(cv::Size boardSize, float squareSize, terraclear::Pattern patternType);
            void computeC2MC1(const cv::Mat &R1, const cv::Mat &tvec1, const cv::Mat &R2, const cv::Mat &tvec2, cv::Mat &R_1to2, cv::Mat &tvec_1to2);
            cv::Mat init_transform();

        private:
            cv::Mat _img;
            cv::Mat cameraMatrix;
            cv::Mat distCoeffs; 
            std::vector<cv::Point3f> objectPoints;
            std::vector<cv::Point2f> corners;  
    };
}

#endif /* CAMERACALIBRATION_H */

