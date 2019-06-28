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

#include "camera_async.hpp"

namespace  terraclear
{  

    camera_async::camera_async(camera_base* pcam, uint32_t fps_max, bool delay_enabled) 
    {
        _delay_enabled = delay_enabled;
        _fps_max = fps_max;
        _pcam = pcam;
        
        //get at least 1 frame and set camera frame ref.
        _pcam->update_frames();
        _buffer_camera = pcam->getRGBFrame();
        
        //set camera mutex to use for frame updates
        _pcam->set_mutex_ptr(&_mutex);
        
        _sw.start();
    }

    camera_async::~camera_async() 
    {
        
    }
    
    camera_base* camera_async::get_basecam_ptr()
    {
        return _pcam;
    }

    uint32_t camera_async::get_fps()
    {
        return _fps_current;
    }
    
    uint32_t camera_async::get_fpsmax()
    {
        return _fps_max;
    }
    
    cv::Mat camera_async::get_ImageBuffer()
    {
        mutex_lock();
           _buffer_back.copyTo(_buffer_front);
        mutex_unlock();

        return _buffer_front;
    }
    
    void camera_async::reset_lost_frames()
    { 
        mutex_lock();
        _lost_frames = 0;
         mutex_unlock();
    }
    
    uint64_t camera_async::get_lost_frames()
    {
        
        return _lost_frames;
    }
    
    void camera_async::thread_runloop()
    {
        //read frame...
        if (_pcam->update_frames())
        {
            int ms_left = (1000 / _fps_max) - _sw.get_elapsed_ms();

            //match requested frame rate if delay enabled
            //i.e. file based cameras.
            if (_delay_enabled && (ms_left > 0))
                usleep(ms_left * 1000);

            mutex_lock();
                uint64_t ms_elapsed = _sw.get_elapsed_ms();
                uint32_t fps_tmp = (ms_elapsed > 0) ? 1000 / ms_elapsed : _fps_current;
                _sw.reset();
                _buffer_camera.copyTo(_buffer_back);
                _fps_current = fps_tmp;
            mutex_unlock();            
        }
        else
        {
            mutex_lock();
                _lost_frames++;
             mutex_unlock();
        }
    }

}