/*
 * Base C++ cross platform camera class 
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

#ifndef CAMERA_BASE_HPP
#define CAMERA_BASE_HPP

#include <opencv2/opencv.hpp>
#include <mutex>

#include "filetools.hpp"

namespace terraclear
{    
    class camera_base 
    {
        public:
            camera_base();
            virtual ~camera_base();

            virtual bool    frame_update() = 0;
            cv::Mat         getRGBFrame();
            virtual void    frame_save(std::string frame_name, bool generate_file_seq = true);
            virtual void    change_white_balance(float red_level, float blue_level);
            virtual void    change_gain(float gain_level);
            virtual void    change_exposure_time(int time);
            virtual void    change_black_level(float level);
            virtual void    change_gamma(float gamma);

            //camera syncronization features
            //caller must supply pointer to mutex.else internal mutex will be used.
            void set_mutex_ptr(std::mutex* mutex_ptr);
            std::mutex* get_mutex_ptr();
            void mutex_lock();
            void mutex_unlock();
            
            std::string get_last_error();

        protected:
            cv::Mat _frame_color;
            std::mutex* _mutex_ptr = nullptr;
            std::mutex _mutex_internal;
            
            std::string _last_error;

        private:

    };
}
#endif /* CAMERA_BASE_HPP */

