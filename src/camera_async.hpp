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

#ifndef CAMERA_ASYNC_HPP
#define CAMERA_ASYNC_HPP

namespace  terraclear
{  
    class camera_async : public thread_base
    {
        public:
            camera_async(camera_base* pcam, uint32_t max_fps = 30);
            virtual ~camera_async();

            uint32_t get_fps();
            uint32_t get_fpsmax();
            cv::Mat get_ImageBuffer();
          
        protected:
            //pure virtual function implementation..
            virtual void thread_runloop();
            
        private:            
            camera_base* _pcam = nullptr;
            uint32_t _fps_max = 30;
            uint32_t _fps_current = 0;
            cv::Mat _camerabuffer;
            cv::Mat _imagebuffer;
            
            stopwatch _sw;
    };
}

#endif /* CAMERA_ASYNC_HPP */

