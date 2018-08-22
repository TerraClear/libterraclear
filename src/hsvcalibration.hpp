
#ifndef HSVCALIBRATION_HPP
#define HSVCALIBRATION_HPP

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

namespace terraclear
{

    class hsvcalibration 
    {
        public:
            hsvcalibration(int &h_low, int &h_high, int &s_low, int &s_high, int &v_low, int &v_high, std::string window_name = "Color Calibration");
            void setimage(cv::Mat diplay_image);
            virtual ~hsvcalibration();

        private:
            cv::Mat _window_img;
            std::string _window_name;

    };

}

#endif /* HSVCALIBRATION_HPP */

