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

#include "detector_hsv.hpp"
#include <opencv2/core/cvstd.hpp>

//#include "/data/sources/yolotracker/Line.hpp"
//#include <opencv>
#include<opencv2/aruco/charuco.hpp>
//#include "/data/sources/yolotracker/Line.hpp"
#include <vector>
#include <iostream>
#include<opencv2/highgui.hpp>

using namespace cv;
namespace terraclear
{
    
    detector_hsv::detector_hsv(cv::Mat imgsrc) 
        : detector_base(imgsrc) 
    {
    }

    detector_hsv::~detector_hsv() 
    {
    }

    std::vector<std::vector<cv::Vec3d>>  detector_hsv::detect_objects() 
    {
        //ret vector
        cv::Vec3d r = (0,0,0);
        
         std::vector<cv::Vec3d> rvec2(3,r);
          std::vector<cv::Vec3d> tvec2(3,r);
        std::vector<bounding_box> ret_vect;
        std::vector<std::vector<cv::Vec3d>> ret_cup;
        cv::Mat mat_filtered;
        cv::Mat mat_thresh;
        //blur Image a bit first.
       cv::blur(_imgsrc, mat_filtered, cv::Size(5,5));
        /// Transform it to HSV color space
        cv::cvtColor(mat_filtered, mat_filtered, cv::COLOR_BGR2HSV);
       // cv::imshow("a",mat_filtered);
        cv::Mat maskHSV;
        cv::Mat resultHSV;
        //find all objects in range
        cv::inRange(mat_filtered, _lowrange, _highrange, maskHSV);
        cv::bitwise_and(mat_filtered,mat_filtered, resultHSV, maskHSV);
        

      //  cv::cvtColor(mat_filtered,mat_filtered,cv::COLOR_RGB2GRAY);
      //  cv::blur(mat_filtered,mat_filtered,cv::Size(3,3));


        mat_filtered = maskHSV;
        cv::Canny(mat_filtered,mat_filtered,100,200);

        //morphological opening (remove small objects from the foreground)
        //cv::erode(mat_filtered, mat_filtered, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2, 2)) );
        cv::dilate( mat_filtered, mat_filtered, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7, 7)) ); 

         //morphological closing (fill small holes in the foreground)
       // cv::dilate( mat_filtered, mat_filtered, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(9, 9)) ); 
       // cv::erode(mat_filtered, mat_filtered, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(9, 9)) );
      //  cv::waitKey(0);
      //  cv::destroyWindow("img");
        char window_name[] = "g";
        cv::namedWindow(window_name, cv::WINDOW_NORMAL | cv::WINDOW_FREERATIO);
        cv::imshow("g",mat_filtered);
        //Vector for all contours.
        //cv::Mat dispIm;
       // dispIm = mat_filtered;
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(mat_filtered, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        //cv::drawContours(dispIm,contours,-1,(0,0,255),CV_FILLED);
       
        int k = 0;
        //create bounding boxes from contours
        int trackid = 0; //unique ID per bounding box..
       //// std::cout << cv::contours.size();
        //for (size_t idx = 0; idx < contours.size(); idx++) {
       /// cv::drawContours(mat_filtered, contours, idx, (255,0,0),-1);
        
     //  std::cout<<"hi"; 
        // strt
          //  c++ cv::VideoCapture inputVideo; inputVideo.open(0);

cv::Mat cameraMatrix, distCoeffs; // camera parameters are read from somewhere 
 //bool detector_hsv::readCameraParameters(std::string flename, cv::Mat &camMatrix, cv::Mat &distCoeffs) {
    cv::FileStorage fs("/data/sources/libterraclear/src/out_chess.xml", FileStorage::READ);
    if(!fs.isOpened()){
        std::cout<<"nooo0";
    };
     //   return false;
    fs["camera_matrix"] >> cameraMatrix;
    fs["distortion_coefficients"] >> distCoeffs;
  //  std::cout<<cameraMatrix;\
 //   std::cout<<distCoeffs;
  //  return true;
    //std::cout<<cameraMatrix;
   // std::cout<<distCoeffs;
//readCameraParameters("out_c5.xml",cameraMatrix, distCoeffs);

//const cv::Ptr<cv::aruco::Dictionary> dictionary1 = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50);
const cv::Ptr<cv::aruco::Dictionary> dictionary2 = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50);
//const cv::Ptr<cv::aruco::CharucoBoard> board2 = cv::aruco::CharucoBoard::create(4, 4, 0.015f, 0.005f,dictionary2);
//const cv::Ptr<cv::aruco::CharucoBoard> board1 = cv::aruco::CharucoBoard::create(5, 5, 0.04f, 0.02f,dictionary1);
//while (inputVideo.grab()) { cv::Mat image, imageCopy; inputVideo.retrieve(image); 
cv::Mat imageCopy;
cv::Mat xyz;
_imgsrc.copyTo(imageCopy);
    //cv::blur(_imgsrc, imageCopy, cv::Size(5,5));

 /*
std::vector<int> ids1; 
std::vector<std::vector<cv::Point2f> > corners1; 
cv::aruco::detectMarkers(imageCopy, dictionary1, corners1, ids1);
*/
std::vector<int> ids2; 
std::vector<std::vector<cv::Point2f> > corners2; 
//cv::aruco::DetectorParameters params; 
//params.doCornerRefinement = false;
//cv::aruco::detectMarkers(inputImage, board.dictionary, markerCorners, markerIds, params);
cv::aruco::detectMarkers(imageCopy, dictionary2, corners2, ids2);
// if at least one marker detected if (ids.size() > 0) { std::vector<cv::Point2f> charucoCorners; std::vector<int> charucoIds; cv::aruco::interpolateCornersCharuco(corners, ids, image, board, charucoCorners, charucoIds, cameraMatrix, distCoeffs); // if at least one charuco corner detected if(charucoIds.size() > 0) { cv::aruco::drawDetectedCornersCharuco(imageCopy, charucoCorners, charucoIds, cv::Scalar(255, 0, 0)); cv::Vec3d rvec, tvec; bool valid = cv::aruco::estimatePoseCharucoBoard(charucoCorners, charucoIds, board, cameraMatrix, distCoeffs, rvec, tvec); // if charuco pose is valid if(valid) cv::aruco::drawAxis(imageCopy, cameraMatrix, distCoeffs, rvec, tvec, 0.1); } }
  if (/*ids1.size() > 0&&*/ids2.size() > 0) {
     // std::cout << "ok";
       /* std::vector<cv::Point2f> charucoCorners1;
        std::vector<int> charucoIds1;
        cv::aruco::interpolateCornersCharuco(corners1, ids1, imageCopy, board1, charucoCorners1, charucoIds1, cameraMatrix, distCoeffs);
        // if at least one charuco corner detected*/
         /* std::vector<cv::Point2f> charucoCorners2;
        std::vector<int> charucoIds2;*/
   //     cv::aruco::interpolateCornersCharuco(corners2, ids2, imageCopy, board2, charucoCorners2, charucoIds2, cameraMatrix, distCoeffs);

  //      if(/*charucoIds1.size() > 0 &&*/charucoIds2.size()>0) {
         //   cv::aruco::drawDetectedCornersCharuco(imageCopy, charucoCorners1, charucoIds1, cv::Scalar(255, 0, 0));
    //        cv::aruco::drawDetectedCornersCharuco(_imgsrc, charucoCorners2, charucoIds2, cv::Scalar(255, 0, 0));
     cv::aruco::drawDetectedMarkers(_imgsrc,corners2,ids2);
      
  
       //     cv::Vec3d rvec1, tvec1;
         
            cv::aruco::estimatePoseSingleMarkers(corners2,.12f, cameraMatrix,distCoeffs,rvec2,tvec2);
       //     std::cout<<std::to_string(ids2.at(0));
       //     bool valid1 = cv::aruco::estimatePoseCharucoBoard(charucoCorners1, charucoIds1, board1, cameraMatrix, distCoeffs, rvec1, tvec1);
            // if charuco pose is valid
  //    bool valid2 = cv::aruco::estimatePoseCharucoBoard(charucoCorners2, charucoIds2, board2, cameraMatrix, distCoeffs, rvec2, tvec2);

          //  if(/*valid1&&*/valid2){
               // cv::aruco::drawAxis(_imgsrc, cameraMatrix, distCoeffs, rvec1, tvec1, 0.1);
         
         /* for(int i=0; i < ids2.size(); i++)
            {
                cv::aruco::drawAxis(_imgsrc, cameraMatrix, distCoeffs,
                        rvec2[i], tvec2[i], 0.1);
          }*/
           // }
  }else{
      //
  }
double x = tvec2[0][0];
double y = tvec2[0][1];
double z = tvec2[0][2];
//std::cout<<"new"<<std::endl;
//std::cout<<std::to_string(x)<<std::endl;
//std::cout<<std::to_string(y)<<std::endl;
//std::cout<<std::to_string(z)<<std::endl;
  ret_cup.push_back(rvec2);
  ret_cup.push_back(tvec2);
  //ret_cup.push_back
         //   char wndow_name[] = "out";
       // cv::namedWindow(wndow_name, cv::WINDOW_NORMAL | cv::WINDOW_FREERATIO);
        //  cv::imshow("out", imageCopy);
        
    

//char key = (char) cv::waitKey(0);
//if (key == 27) break; 
        //emd
          //drawDetectedCornersCharuco
    //}
        int big_track = 0;
        std::vector<cv::Point> largest_cont;
        for (auto contour : contours)
        {
            trackid++;
            if (trackid == 1){
                largest_cont = contour;
            }
            if(cv::contourArea(contour)>100){
            cv::Rect rect = cv::boundingRect(cv::Mat(contour));
            bounding_box bbox;
            bbox.x = rect.x;
            bbox.y = rect.y;
            bbox.width = rect.width;
            bbox.height = rect.height;
            bbox.class_id = 0;
            bbox.confidence = 1.0f;
            bbox.track_id = trackid;
            bbox.frame_count = 0;
           // std::cout <<bbox.x;
            ret_vect.push_back(bbox);
            //cv::drawContours(_imgsrc, contours, k, (255,255,0),-1);
            if(cv::contourArea(contour)>cv::contourArea(largest_cont)){
                largest_cont = contour;
                big_track = trackid;
                //std::cout<<cv::contourArea(contour);
            }
            }
            k++;
            ///home/koos/Desktop/Dropbox/Private/Code/cpp/yolotracker

        }  
      //  cv::drawContours(_imgsrc,contours,big_track-1,(255,255,0),-1);
        if(big_track != 0){
      //  std::cout<<cv::contourArea(largest_cont);
            
        }
     //    char window[] = "img";
        //cv::namedWindow(window, cv::WINDOW_NORMAL | cv::WINDOW_FREERATIO);

        //cv::imshow("img",mat_filtered);
        //cluster into larger boxes
      mergeBoundingBoxes(ret_vect, 60);
        
        //TODO: iterate through until no overlaps..
        //twice seems to do the trick..
       mergeBoundingBoxes(ret_vect, 60);

     
       return ret_cup;


}
}
