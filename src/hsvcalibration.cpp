
#include "hsvcalibration.hpp"

namespace terraclear
{
    
    hsvcalibration::hsvcalibration(int &h_low, int &h_high, int &s_low, int &s_high, int &v_low, int &v_high, std::string window_name) 
    {
        //Open CV Window stuff
        _window_name = window_name;
        cv::namedWindow(window_name, cv::WINDOW_NORMAL|cv::WINDOW_AUTOSIZE);
        
        cv::createTrackbar("HL", _window_name, &h_low, 175, nullptr);
        cv::createTrackbar("HH", _window_name, &h_high, 175, nullptr);
        cv::createTrackbar("SL", _window_name, &s_low, 255, nullptr);
        cv::createTrackbar("SH", _window_name, &s_high, 255, nullptr);
        cv::createTrackbar("VL", _window_name, &v_low, 255, nullptr);
        cv::createTrackbar("VH", _window_name, &v_high, 255, nullptr);

        _window_img = cv::Mat1s(1,1);
        cv::imshow(window_name, _window_img);
    }

    hsvcalibration::~hsvcalibration() 
    {
    }
    
     void hsvcalibration::setimage(cv::Mat diplay_image)
     {
         if (diplay_image.data)
         {
             _window_img = diplay_image;
            cv::imshow(_window_name, _window_img);
         }
     }
}


