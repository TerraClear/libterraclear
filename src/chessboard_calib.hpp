/*
 *  Copyright: TerraClear, Inc - 2019
 *  Author: Alex Witt (alex.witt@terraclear.com)
 */

#ifndef CHESSBOARD_CALIB_H
#define CHESSBOARD_CALIB_H

#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/calib3d.hpp>

namespace  terraclear
{
    enum Pattern { CHESSBOARD, CIRCLES_GRID, ASYMMETRIC_CIRCLES_GRID };
    
    class chessboard_calib
    {
        public:

            chessboard_calib(std::string camera_xml);
            void update_frame(const cv::Mat img_src);
            bool findChessBoard(const cv::Size board_sz);
            void addManualCorners(std::vector<cv::Point2f> internal_corners);
            void calcChessboardCorners(cv::Size boardSize, terraclear::Pattern patternType);
            void computeC2MC1(const cv::Mat &R1, const cv::Mat &tvec1, const cv::Mat &R2, const cv::Mat &tvec2, cv::Mat &R_1to2, cv::Mat &tvec_1to2);
            cv::Mat init_transform();
            void save_mat(std::string cfg_filename);
            cv::Mat        _transform_matrix;
            float          _block_size;
            bool           _board_found;
            float          _gsd;

        private:   
            cv::Mat _img;
            cv::Mat cameraMatrix;
            cv::Mat distCoeffs; 
            std::vector<cv::Point3f> objectPoints;
            std::vector<cv::Point2f> corners; 
    };
}

#endif /* CHESSBOARD_CALIB_H */

