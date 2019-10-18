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
        _pcam->frame_update();
        _buffer_camera = pcam->getRGBFrame();
        
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
    
    uint64_t camera_async::get_total_frame_count()
    {
        return _total_frame_count;
    }
    
    frame_info camera_async::get_ImageBuffer()
    {
        frame_info new_frame;
        
        mutex_lock();
           _buffer_back.copyTo(_buffer_front);
        mutex_unlock();

        new_frame.cam_frame = _buffer_front;
        new_frame.resting_time = _sw.get_elapsed_ms();
        
        return new_frame;
    }
    
    void camera_async::reset_lost_frames()
    { 
        mutex_lock();
        _lost_frames = 0;
         mutex_unlock();
    }
    
    void camera_async::change_white_balance(float red_level, float blue_level)
    {
        _pcam->change_white_balance(red_level, blue_level);
    }
    
    void camera_async::change_gain(float gain_level)
    {
        _pcam->change_gain(gain_level);
    }
    
    void camera_async::change_exposure_time(int time)
    {
        _pcam->change_exposure_time(time);
    }
    
    void camera_async::change_black_level(float level)
    {
        _pcam->change_black_level(level);
    }
    
    void camera_async::change_gamma(float gamma)
    {
        _pcam->change_gamma(gamma);
    }
    
    uint64_t camera_async::get_lost_frames()
    {
        
        return _lost_frames;
    }
    
    void camera_async::thread_runloop()
    {
        //read frame...
        if (_pcam->frame_update())
        {
            _total_frame_count++;
            
            int ms_left = (1000 / _fps_max) - _sw.get_elapsed_ms();

            //match requested frame rate if delay enabled
            //i.e. file based cameras.
            if (_delay_enabled && (ms_left > 0))
                usleep(ms_left * 1000);

            mutex_lock();
                //update FPS every 1sec
                _ms_elapsed += _sw.get_elapsed_ms();
                _frame_count ++;
                if (_ms_elapsed  >= 1000)
                {
                    _fps_current =  std::round((float)_frame_count * ( 1000 / (float)_ms_elapsed));
                    _frame_count = _ms_elapsed = 0;
                }

            //copy current camera frame to back buffer.
            _buffer_camera.copyTo(_buffer_back);

            _sw.reset();
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