/*
 * base class for calibration 
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

#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui.hpp>

#include "hsvcalibration.hpp"

namespace terraclear
{
    
    //static vars..
    cv::Mat hsvcalibration::_window_img;
    std::string hsvcalibration::_window_name;
            
    hsvcalibration::hsvcalibration(int &h_low, int &h_high, int &s_low, int &s_high, int &v_low, int &v_high, std::string window_name) 
    {

        _h_low = &h_low;
        _h_high = &h_high;
        _s_low = &s_low;
        _s_high = &s_high;
        _v_low = &v_low;
        _v_high = &v_high;
        
        //Open CV Window stuff
        _window_name = window_name;
        
        cv::namedWindow(window_name, cv::WINDOW_NORMAL|cv::WINDOW_AUTOSIZE);
        
        _window_img = cv::Mat(50, 100, CV_8UC3, cv::Scalar(0xff, 0x00, 0x00));

        cv::createTrackbar("HL", window_name, _h_low, 255, hsvcalibration::callback_slider, this);
        cv::createTrackbar("HH", window_name, _h_high, 255, hsvcalibration::callback_slider, this);
        cv::createTrackbar("SL", window_name, _s_low, 255, hsvcalibration::callback_slider, this);
        cv::createTrackbar("SH", window_name, _s_high, 255, hsvcalibration::callback_slider, this);
        cv::createTrackbar("VL", window_name, _v_low, 255, hsvcalibration::callback_slider, this);
        cv::createTrackbar("VH", window_name, _v_high, 255, hsvcalibration::callback_slider, this);

        fill_hsv();
    }

    hsvcalibration::~hsvcalibration() 
    {
    }
    
    void hsvcalibration::callback_slider(int track_val, void* user_param)
    {
        hsvcalibration* this_ptr = (hsvcalibration*)(user_param);
        this_ptr->fill_hsv();
    }
     
    void hsvcalibration::fill_hsv()
    {
        cv::Mat tmp_hsv = cv::Mat(_window_img.rows,_window_img.cols, CV_8UC3, cv::Scalar(0xff, 0x00, 0x00));
        cv::cvtColor(_window_img, tmp_hsv, CV_BGR2HSV);
        cv::Rect rect1(0,0,50, 50);        
        cv::Rect rect2(_window_img.cols/2, 0, _window_img.cols, _window_img.rows);        
        cv::rectangle(tmp_hsv, rect1, cv::Scalar(*_h_low, *_s_low, *_v_low), -1);
        cv::rectangle(tmp_hsv, rect2, cv::Scalar(*_h_high, *_s_high, *_v_high), -1);
        cv::cvtColor(tmp_hsv, _window_img, CV_HSV2BGR);

        cv::imshow(_window_name, _window_img);
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


