/*
 * basic hsv color detector
 * Copyright (C) 2019 TerraClear, Inc.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 * CREATED BY: Koos du Preez - koos@terraclear.com
 * 
*/

#ifndef DETECTOR_HSV_HPP
#define DETECTOR_HSV_HPP

#include "detector_base.hpp"
#include<opencv2/aruco/charuco.hpp>
//#include "/data/sources/yolotracker/Line.hpp"
#include <vector>
#include <iostream>
#include<opencv2/highgui.hpp>
//using namespace cv
namespace terraclear
{

    class detector_hsv : public detector_base
    {
        public:
            //default values for various shades of blue.
            //19,65,100
            //150,300,300
            cv::Scalar _lowrange = cv::Scalar(0,0,0 );
            cv::Scalar _highrange = cv::Scalar(10,10,10);
            
            //cv::Scalar _lowrange = cv::Scalar(87, 155, 120 );
           // cv::Scalar _highrange = cv::Scalar(109,249,254);
            detector_hsv(cv::Mat imgsrc);
            virtual ~detector_hsv();
           //  static bool readDetectorParameters(std::string filename, cv::Ptr<cv::aruco::DetectorParameters> &params);
          //  static bool readCameraParameters(std::string flename, cv::Mat &camMatrix, cv::Mat &distCoeffs);

            //pure virtual implementation
            std::vector<std::vector<cv::Vec3d>>  detect_objects();
            double calc_area();
            

        private:
            double marker_size;
            

    };
}
#endif /* DETECTOR_HSV_HPP */

