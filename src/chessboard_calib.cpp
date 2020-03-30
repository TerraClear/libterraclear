/*
 *  Copyright: TerraClear, Inc - 2019
 *  Author: Alex Witt (alex.witt@terraclear.com)
 */
#include <iostream>
#include <jsoncpp/json/json.h>

#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/calib3d.hpp>

#include "chessboard_calib.hpp"
#include "filetools.hpp"

namespace  terraclear
{   
    chessboard_calib::chessboard_calib(std::string camera_xml) 
    {   
        // Read in camera intrinsics 
        cv::FileStorage fs(camera_xml, cv::FileStorage::READ);
        fs["camera_matrix"] >> cameraMatrix;
        fs["distortion_coefficients"] >> distCoeffs;
    }
        
    void chessboard_calib::update_frame(const cv::Mat img_src)
    {
        cv::cvtColor(img_src, _img, cv::COLOR_RGB2GRAY);
    }

    bool chessboard_calib::findChessBoard(const cv::Size board_sz) {
        // Find chess board corners 
        return cv::findChessboardCorners(_img, board_sz, corners);
    }
    
    void chessboard_calib::addManualCorners(std::vector<cv::Point2f> internal_corners)
    {
        for (int i = 0; i < internal_corners.size(); i++)
        {
            corners.push_back(internal_corners.at(i));
        }
    }
    
    void chessboard_calib::calcChessboardCorners(cv::Size boardSize, terraclear::Pattern patternType = terraclear::Pattern::CHESSBOARD)
    {
        objectPoints.resize(0);
        
        switch (patternType)
        {
            case CHESSBOARD:
            case CIRCLES_GRID:
                //! [compute-chessboard-object-points]
                for( int i = 0; i < boardSize.height; i++ )
                    for( int j = 0; j < boardSize.width; j++ )
                        //To try to center the chessboard frame, we substract the image size
                        objectPoints.push_back(cv::Point3f(float((j-boardSize.width/2)*_block_size),
                                                float((i-boardSize.height/2)*_block_size), 0));
                //! [compute-chessboard-object-points]
                break;

            case ASYMMETRIC_CIRCLES_GRID:
                for( int i = 0; i < boardSize.height; i++ )
                    for( int j = 0; j < boardSize.width; j++ )
                        objectPoints.push_back(cv::Point3f(float((2*j + i % 2)*_block_size),
                                                float(i*_block_size), 0));
                break;

            default:
                CV_Error(cv::Error::StsBadArg, "Unknown pattern type\n");
        }
    }

    void chessboard_calib::computeC2MC1(const cv::Mat &R1, const cv::Mat &tvec1, const cv::Mat &R2, const cv::Mat &tvec2,
                      cv::Mat &R_1to2, cv::Mat &tvec_1to2)
    {
        R_1to2 = R2 * R1.t();
        tvec_1to2 = R2 * (-R1.t()*tvec1) + tvec2;
    }
    
    cv::Mat chessboard_calib::init_transform()
    {
        cv::Mat rvec, tvec;
        
        cv::solvePnP(objectPoints, corners, cameraMatrix, distCoeffs, rvec, tvec);
        
        cv::Mat R_desired = (cv::Mat_<double>(3,3) <<
                        0, -1, 0,
                        1, 0, 0,
                        0, 0, 1);
        cv::Mat R;
        cv::Rodrigues(rvec, R);
        cv::Mat normal = (cv::Mat_<double>(3,1) << 0, 0, 1);
        cv::Mat normal1 = R*normal;
        cv::Mat origin(3, 1, CV_64F, cv::Scalar(0));
        cv::Mat origin1 = R*origin + tvec;
        
        double d_inv1 = 1.0 / normal1.dot(origin1);
        
        cv::Mat R_1to2, tvec_1to2;
        cv::Mat tvec_desired = tvec.clone();
        chessboard_calib::computeC2MC1(R, tvec, R_desired, tvec_desired, R_1to2, tvec_1to2);
        
        _transform_matrix = R_1to2 + d_inv1 * tvec_1to2*normal1.t();
        _transform_matrix = cameraMatrix * _transform_matrix * cameraMatrix.inv();
        cv::Mat final_mat = _transform_matrix/_transform_matrix.at<double>(2,2);
        std::vector<cv::Point2f> worldPoints;
        
        cv::perspectiveTransform(corners, worldPoints,final_mat);
        _gsd = _block_size / abs(worldPoints[0].y - worldPoints[1].y);

        return final_mat;
            
    }

    void chessboard_calib::save_mat(std::string cfg_filename)
    {
        //* Write the new transformation matrix back into the .json cfg *//
        Json::Value json_file;
        std::ifstream proximal_info(cfg_filename, std::ifstream::binary);
        proximal_info >> json_file;

        // Writer for new file
        Json::StyledWriter styledWriter;
        Json::Value new_file;
        
        // Open cfg for writing over
        std::ofstream file_id;
        file_id.open(cfg_filename);
        // Go through the cfg looking for the homography matrix section
        for (auto iter = json_file.begin(); iter != json_file.end(); iter++)
        {   
            new_file[iter.key().asCString()] = *iter;
            
            if (iter.key() == "perspective-correction")
            {
                Json::Value content(Json::arrayValue);
                this->_transform_matrix = this->init_transform();
                cv::Mat reshape_mat = this->_transform_matrix.reshape(0,9);
                for (int i = 0; i != reshape_mat.rows; i++)
                {
                    content.append(reshape_mat.at<double>(0,i));
                }
                new_file["perspective-correction"]["mat"] = content;
                // add in new gsd calculation 
                new_file["perspective-correction"]["gsd-mm-px"] = this->_gsd;
                // set the cfg to enable perspective correction
                new_file["perspective-correction"]["enable"] = true;
                // set the cfg to disable chessboard looking
                new_file["perspective-correction"]["check-chessboard"] = false;
            }
        }
        // Write modified json
        file_id << styledWriter.write(new_file); 
    }

}
