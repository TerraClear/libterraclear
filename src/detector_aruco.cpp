
#include "detector_aruco.hpp"
#include <opencv2/core/cvstd.hpp>

//#include "/data/sources/yolotracker/Line.hpp"
//#include <opencv>
#include<opencv2/aruco/charuco.hpp>
//#include "/data/sources/yolotracker/Line.hpp"
#include <vector>
#include <iostream>
#include<opencv2/highgui.hpp>

using namespace cv;
namespace terraclear {
    std::vector<std::vector<cv::Point2f> > corners2;
    cv::Mat cameraMatrix, distCoeffs;

    detector_aruco::detector_aruco(cv::Mat imgsrc)
    /*: detector_base(imgsrc) */{

        cv::FileStorage fs("/data/sources/libterraclear/src/flir_calib.xml", FileStorage::READ);
        if (!fs.isOpened()) {
            std::cout << "nooo0";
        };
        //   return false;
        fs["camera_matrix"] >> cameraMatrix;
        fs["distortion_coefficients"] >> distCoeffs;

    }

    detector_aruco::~detector_aruco() {
    }

    paddles detector_aruco::detect_objects() //format:element 0 of output is rvec. element one is tvec. rvec and tvec are arrays of 
    //vec3d's , one for each detected marker
    {

        //ret vector/rvec2 = (3,r);
        //tvec2 = (3,r);
        cv::Vec3d rstart = (0, 0, 0);
        std::vector<cv::Vec3d> rvec2(3, rstart);
        std::vector<cv::Vec3d> tvec2(3, rstart);
        //  std::vector<bounding_box> ret_vect;
        // std::vector<std::vector<cv::Point2f> > corners2; 
        std::vector<int> ids2;
        paddles found_markers;
        found_markers.rvec = rvec2;
        found_markers.tvec = tvec2;
        std::vector<cv::Point3d> pts(3,cv::Point3d(0,0,0));
        found_markers.real_coords = pts;
        found_markers.num_paddles = 0;
        const cv::Ptr<cv::aruco::Dictionary> dictionary2 = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50);

        cv::Mat imageCopy;
        cv::Mat xyz;
        _imgsrc.copyTo(imageCopy);

        cv::aruco::detectMarkers(imageCopy, dictionary2, corners2, ids2);
        // if at least one marker detected if (ids.size() > 0) { std::vector<cv::Point2f> charucoCorners; std::vector<int> charucoIds; cv::aruco::interpolateCornersCharuco(corners, ids, image, board, charucoCorners, charucoIds, cameraMatrix, distCoeffs); // if at least one charuco corner detected if(charucoIds.size() > 0) { cv::aruco::drawDetectedCornersCharuco(imageCopy, charucoCorners, charucoIds, cv::Scalar(255, 0, 0)); cv::Vec3d rvec, tvec; bool valid = cv::aruco::estimatePoseCharucoBoard(charucoCorners, charucoIds, board, cameraMatrix, distCoeffs, rvec, tvec); // if charuco pose is valid if(valid) cv::aruco::drawAxis(imageCopy, cameraMatrix, distCoeffs, rvec, tvec, 0.1); } }
        if (ids2.size() > 0) {
            found_markers.real_coords.clear();
            cv::aruco::drawDetectedMarkers(_imgsrc, corners2, ids2);

            cv::aruco::estimatePoseSingleMarkers(corners2, .12f, cameraMatrix, distCoeffs, rvec2, tvec2);
            found_markers.num_paddles = ids2.size();
            found_markers.rvec = rvec2;
        found_markers.tvec = tvec2;
        }
      
        for(int i=0; i<tvec2.size();i++){
       // double x = tvec2[0][0];
       // double y = tvec2[0][1];
       // double z = tvec2[0][2];
            cv::Vec3d pointy = tvec2[i];
            cv::Point3d found = cv::Point3d(pointy[0],pointy[1],pointy[2]);
            found_markers.real_coords.push_back(found);
            
        }
      //  std::cout<<"wanna b x "<<std::to_string(found_markers.tvec[0][0])<<std::endl;

        return found_markers;

    }
    


    double detector_aruco::calc_area() {
        double area = 0;
        if (corners2.size() != 0) {
            cv::Point2f top_l = corners2.at(0)[0];
            cv::Point2f top_r = corners2.at(0)[1];
            cv::Point2f bot_r = corners2.at(0)[2];
            double l1 = std::abs(top_l.x - top_r.x);
            double l2 = std::abs(top_r.y - bot_r.y);
            area = l1*l2;
            std::cout << std::flush;
        }
        //  std::cout<<"size "<<std::to_string(area)<<std::endl;


        // std::cout<<"size "<<std::to_string((corners2.at(0)).size())<<std::endl;
        return area;
    }



}

