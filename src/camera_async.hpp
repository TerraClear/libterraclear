/*
 * Async Threaded C++ cross platform camera class 
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

#include "thread_base.hpp"
#include "camera_base.hpp"
#include "stopwatch.hpp"

#include <opencv2/opencv.hpp>
#include <opencv2/core/hal/interface.h>

#ifndef CAMERA_ASYNC_HPP
#define CAMERA_ASYNC_HPP

struct frame_info
{
    cv::Mat cam_frame;
    uint64_t resting_time;
};

namespace  terraclear
{  
    class camera_async : public thread_base
    {
        public:
            camera_async(camera_base* pcam, uint32_t max_fps = 30, bool delay_enabled = false);
            virtual ~camera_async();

            uint32_t get_fps();
            uint32_t get_fpsmax();
            uint64_t get_total_frame_count();
            camera_base* get_basecam_ptr();
            frame_info get_ImageBuffer();
          
            uint64_t get_lost_frames();
            void reset_lost_frames();
            void change_white_balance(float red_level, float blue_level);
            void change_gain(float gain_level);
            void change_exposure_time(int time);
            void change_black_level(float level);
            void change_gamma(float gamma);
            
        protected:
            //pure virtual function implementation..
            virtual void thread_runloop();
            
        private:         
            uint64_t _lost_frames = 0;
            
            bool _delay_enabled = false;
            camera_base* _pcam = nullptr;
            uint32_t _fps_max = 30;
            uint32_t _fps_current = 0;
            
            uint64_t _ms_elapsed = 0;
            uint32_t _frame_count = 0;
            
            uint64_t _total_frame_count = 0;
            
            cv::Mat _buffer_camera;
            cv::Mat _buffer_back;
            cv::Mat _buffer_front;
            
            stopwatch _sw;
    };
}

#endif /* CAMERA_ASYNC_HPP */

