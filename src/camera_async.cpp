/*
 * Async Threaded C++ cross platform camera class 
 * Copyright (C) 2017 TerraClear, Inc.
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

    camera_async::camera_async(camera_base* pcam, uint32_t fps_max) 
    {
        _fps_max = fps_max;
        _pcam = pcam;
        
        //get at least 1 frame and set camera frame ref.
        _pcam->update_frames();
        _camerabuffer = pcam->getRGBFrame();
        
        _sw.start();
    }

    camera_async::~camera_async() 
    {
        
    }

    uint32_t camera_async::get_fps()
    {
        return _fps_current;
    }
    
    uint32_t camera_async::get_fpsmax()
    {
        uint32_t retval = _fps_max;
    }
    
    cv::Mat camera_async::get_ImageBuffer()
    {
        return _imagebuffer;
    }
    
    void camera_async::thread_runloop()
    {
        //read frame...
        _pcam->update_frames(); 

        int ms_left = (1000 / _fps_max) - _sw.get_elapsed_ms();
        
        //match requested frame rate..
        if (ms_left > 0)
            usleep(ms_left * 1000);
        
        mutex_lock();
            _camerabuffer.copyTo(_imagebuffer);
            _fps_current = 1000 / _sw.get_elapsed_ms();
        mutex_unlock();
        
        //sleep for ms to match frame rate.
        _sw.reset();
    }

}