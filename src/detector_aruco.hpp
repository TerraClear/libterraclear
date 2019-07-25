/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   detector_aruco.hpp
 * Author: anishau
 *
 * Created on July 10, 2019, 2:18 PM
 */

#ifndef DETECTOR_ARUCO_HPP
#define DETECTOR_ARUCO_HPP

#include "detector_base.hpp"
#include<opencv2/aruco/charuco.hpp>
//#include "/data/sources/yolotracker/Line.hpp"
#include <vector>
#include <iostream>
#include<opencv2/highgui.hpp>
#include "paddles.h"

namespace terraclear
{

    class detector_aruco// : public detector_base
    {
        public:

            cv::Scalar _highrange = cv::Scalar(10,10,10);
        
            detector_aruco(cv::Mat imgsrc);
            virtual ~detector_aruco();
             //bool readDetectorParameters(std::string filename, Ptr<aruco::DetectorParameters> &params);

            //pure virtual implementation
             // paddles detect_paddles();
              paddles detect_objects();
            double calc_area();
            
         //   std::vector<cv::Point3d> marker_loc_3d();
            

        private:
            cv::Mat _imgsrc;
            double marker_size;
            

    };
}



#endif /* DETECTOR_ARUCO_HPP */
