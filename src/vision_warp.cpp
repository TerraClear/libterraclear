/*
 *  Copyright: TerraClear, Inc - 2019
 *  Author: TerraClear Team
 */

#include <opencv2/core/cuda.hpp>
#include <opencv2/cudaarithm.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudawarping.hpp>
#include <opencv2/calib3d.hpp>

#include "vision_warp.h"
#include <libterraclear/src/filetools.hpp>

using namespace cv;
using namespace std;

namespace  terraclear
{   
    vision_warp::vision_warp(std::string exe_filepath) 
    {
        _sw.start();
        
        // Get path to camera calibration file
        std::string basepath = terraclear::filetools::get_base_path(exe_filepath);
        std::string cam_file = basepath + "/../../../camera.xml";
        // Read in camera intrinsics 
        cv::FileStorage fs(cam_file, FileStorage::READ);
        fs["camera_matrix"] >> cameraMatrix;
        fs["distortion_coefficients"] >> distCoeffs;
    }

    vision_warp::~vision_warp() 
    {
    }
        
    void vision_warp::update_frame(const cv::Mat img_src)
    {
        _img = img_src;
    }
    cv::Mat vision_warp::get_transform_matrix()
    {
        return _transform_matrix;
    }
    
    cv::Mat vision_warp::transform_image(cv::Mat img_src)
    {
        cv::Mat img_result;
        //warp original & resize.
        _sw.reset();
        cv::warpPerspective(img_src, img_result, _transform_matrix, img_src.size()); // do perspective transformation
        _elapsed_us = _sw.get_elapsed_us();
      
        return img_result;
    }
    
    cv::Mat vision_warp::transform_image(cv::Mat img_src, bool flip)
    {
        cv::Mat img_result;
        
        //warp original & resize.
        cv::warpPerspective(img_src, img_result, _transform_matrix, _target_size); // do perspective transformation
            
        _sw.reset();
        if (flip)
        {   
            cv::Mat img_flip;
            cv::flip(img_result, img_flip, 0);
            img_result = img_flip;
        }
        _elapsed_us = _sw.get_elapsed_us();
      
        return img_result;
    }
    
    cv::Mat vision_warp::transform_image_gpu(cv::Mat img_src)
    {
        //copy image to GPU
        cv::cuda::GpuMat gpu_src(img_src);
        cv::cuda::GpuMat gpu_dst;
        
        //warp original & resize.
        _sw.reset();
        cv::cuda::warpPerspective(gpu_src, gpu_dst, _transform_matrix, _target_size); // do perspective transformation
        _elapsed_us = _sw.get_elapsed_us();
        
        //copy GPU image back to regular cv mat
        cv::Mat img_result(gpu_dst);
        
        return img_result;
    }
    
    cv::Mat vision_warp::transform_image_crop(cv::Mat img_src)
    {
        cv::Mat img_result;
        //warp original & resize.
        _sw.reset();
        cv::warpPerspective(img_src, img_result, _transform_matrix, img_src.size()); // do perspective transformation
        
        cv::Rect myROI((img_result.cols /2) - 350, (img_result.rows) - img_result.rows, 700, img_result.rows);
        cv::Mat crop = img_result(myROI);
        cv::Mat resize;
        cv::resize(crop,resize,cv::Size(700,1200),0,0,cv::INTER_CUBIC);
        _elapsed_us = _sw.get_elapsed_us();
      
        return resize;
    }
    
    cv::Mat vision_warp::transform_image_gpu_crop(cv::Mat img_src, cv::Size size)
    {
        //copy image to GPU
        cv::cuda::GpuMat gpu_src(img_src);
        cv::cuda::GpuMat gpu_dst;
        
        //warp original & resize.
        _sw.reset();
        cv::cuda::warpPerspective(gpu_src, gpu_dst, _transform_matrix, _target_size, cv::RANSAC); // do perspective transformation
        cv::Mat img_result(gpu_dst);
        
        cv::Rect myROI((img_result.cols /2) - 350, (img_result.rows) - img_result.rows, 700, img_result.rows);
        cv::Mat crop = img_result(myROI);
        
        _elapsed_us = _sw.get_elapsed_us();
        
        //copy GPU image back to regular cv mat
        
        //cv::Rect rect((img_result.cols - size.width)/2,0,size.width,size.height);
        //cv::Mat crop = img_result(rect);
        return crop;
    }
 
    cv::Mat vision_warp::transform_image_gpu(cv::Mat img_src, bool flip)
    {
        // Initialize final image;
        cv::Mat img_result;
        
        //copy image to GPU
        cv::cuda::GpuMat gpu_src(img_src);
        cv::cuda::GpuMat gpu_dst;
   
        //warp original & resize.
        _sw.reset();
        cv::cuda::warpPerspective(gpu_src, gpu_dst, _transform_matrix, _target_size); // do perspective transformation
        // For flipping negative roll 
        if (flip)
        {
            cv::Mat flip_base;
            cv::cuda::GpuMat flip_gpu;

            flip_gpu.upload(flip_base);
            cv::cuda::flip(gpu_dst, flip_gpu, 0); 
            
            // Copy flipped image back to cpu
            flip_gpu.download(img_result);
            flip_gpu.release();
        }
        else
        {
            // Copy unflipped image back to cpu
            gpu_dst.download(img_result);
            gpu_dst.release();
        }
        _elapsed_us = _sw.get_elapsed_us();

        return img_result;
    } 
    
    cv::Mat_<double> get_dst_corners(std::vector<cv::Point> src, cv::Mat trns)
    {
        // Initialize dst_corners mat
        cv::Mat_<double> dst_corners = (cv::Mat_<double>(4,2) << 0,0,0,0,0,0);  

        for (int i = 0; i < 4; ++i)
        {
            dst_corners.at<double>(i,0) = (double)std::floor((
                                           trns.at<double>(0,0) * src[i].x + 
                                           trns.at<double>(0,1) * src[i].y +
                                           trns.at<double>(0,2))
                                         / 
                                         ( trns.at<double>(2,0) * src[i].x +
                                           trns.at<double>(2,1) * src[i].y +
                                           trns.at<double>(2,2)));

            dst_corners.at<double>(i,1) = (double)std::floor((
                                           trns.at<double>(1,0) * src[i].x  +
                                           trns.at<double>(1,1) * src[i].y  +
                                           trns.at<double>(1,2))
                                         / 
                                         ( trns.at<double>(2,0) * src[i].x  +
                                           trns.at<double>(2,1) * src[i].y  +
                                           trns.at<double>(2,2)));
        }

        return dst_corners;
    }
    
    bool vision_warp::findChessBoard(const cv::Size board_sz) {
        // Find chess board corners 
        return findChessboardCorners(_img, board_sz, corners);
    }
    
    void vision_warp::calcChessboardCorners(cv::Size boardSize, float squareSize, terraclear::Pattern patternType = terraclear::Pattern::CHESSBOARD)
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

    void vision_warp::computeC2MC1(const Mat &R1, const Mat &tvec1, const Mat &R2, const Mat &tvec2,
                      Mat &R_1to2, Mat &tvec_1to2)
    {
        //c2Mc1 = c2Mo * oMc1 = c2Mo * c1Mo.inv()
        R_1to2 = R2 * R1.t();
        tvec_1to2 = R2 * (-R1.t()*tvec1) + tvec2;
    }
    
    cv::Mat vision_warp::init_transform()
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

        vision_warp::computeC2MC1(R, tvec, R_desired, tvec_desired, R_1to2, tvec_1to2);
       
        _transform_matrix = R_1to2 + d_inv1 * tvec_1to2*normal1.t();

        _transform_matrix = cameraMatrix * _transform_matrix * cameraMatrix.inv();
        
        return  _transform_matrix/_transform_matrix.at<double>(2,2);
            
    }
    
}
