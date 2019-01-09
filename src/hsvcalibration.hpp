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

