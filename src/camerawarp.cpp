/* 
 * File:   camerawarp.cpp
 * Author: alexwitt
 * 
 * Created on August 8, 2019, 4:32 PM
 */

#include <opencv2/opencv.hpp>
#include <libterraclear/src/filetools.hpp>
#include "camerawarp.h"

using namespace std;
using namespace cv;

namespace terraclear
{
    camerawarp::camerawarp() {}  
    
    camerawarp::~camerawarp() {}
       
    void camerawarp::get_camera_paramters(std::string filepath) 
    {
        // Get path to camera calibration file
        std::string basepath = terraclear::filetools::get_base_path(filepath);
        std::string cam_file = basepath + "/../../../camera.xml";
        // Read in camera intrinsics 
        cv::FileStorage fs(cam_file, FileStorage::READ);
        fs["camera_matrix"] >> cameraMatrix;
        fs["distortion_coefficients"] >> distCoeffs;
    }
    
    void camerawarp::update_frame(const cv::Mat img_src)
    {
        _img = img_src;
    }
    
    void camerawarp::undistort_img()
    {
        // Undistort image 
        cv::undistort(_img, _undistorted_img, cameraMatrix, distCoeffs);          
    }

    bool camerawarp::findChessBoard(const cv::Size board_sz) {
        // Find chess board corners 
        return findChessboardCorners(_undistorted_img, board_sz, corners);
    }
    
    void camerawarp::calcChessboardCorners(cv::Size boardSize, float squareSize, terraclear::Pattern patternType = terraclear::Pattern::CHESSBOARD)
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
                    objectPoints.push_back(cv::Point3f(float((j-boardSize.width/2)*squareSize),
                                              float((i-boardSize.height/2)*squareSize), 0));
            //! [compute-chessboard-object-points]
            break;

        case ASYMMETRIC_CIRCLES_GRID:
            for( int i = 0; i < boardSize.height; i++ )
                for( int j = 0; j < boardSize.width; j++ )
                    objectPoints.push_back(cv::Point3f(float((2*j + i % 2)*squareSize),
                                              float(i*squareSize), 0));
            break;

        default:
            CV_Error(Error::StsBadArg, "Unknown pattern type\n");
        }
    }

    void camerawarp::computeC2MC1(const Mat &R1, const Mat &tvec1, const Mat &R2, const Mat &tvec2,
                      Mat &R_1to2, Mat &tvec_1to2)
    {
        //c2Mc1 = c2Mo * oMc1 = c2Mo * c1Mo.inv()
        R_1to2 = R2 * R1.t();
        tvec_1to2 = R2 * (-R1.t()*tvec1) + tvec2;
    }
    
    cv::Mat camerawarp::init_transform()
    {
        cv::Mat rvec, tvec;
        cv::solvePnP(objectPoints, corners, cameraMatrix, distCoeffs, rvec, tvec);

        cv::Mat R_desired = (Mat_<double>(3,3) <<
                        -1, 0, 0,
                        0, -1, 0,
                        0, 0, 1);
        cv::Mat R;
        cv::Rodrigues(rvec, R);
        cv::Mat normal = (Mat_<double>(3,1) << 0, 0, 1);
        cv::Mat normal1 = R*normal;
        cv::Mat origin(3, 1, CV_64F, Scalar(0));
        cv::Mat origin1 = R*origin + tvec;
        
        double d_inv1 = 1.0 / normal1.dot(origin1);
        
        cv::Mat R_1to2, tvec_1to2;
        cv::Mat tvec_desired = tvec.clone();

        camerawarp::computeC2MC1(R, tvec, R_desired, tvec_desired, R_1to2, tvec_1to2);
       
        _H = R_1to2 + d_inv1 * tvec_1to2*normal1.t();

        _H = cameraMatrix * _H * cameraMatrix.inv();
        
        return  _H/_H.at<double>(2,2);
    }
}