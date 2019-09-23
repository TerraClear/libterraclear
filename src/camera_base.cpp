/*
 * Base Depth Camera class 
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

#include "camera_base.hpp"

namespace terraclear
{
    camera_base::camera_base() 
    {
    }

    camera_base::~camera_base() 
    {
    }
    
    cv::Mat camera_base::getRGBFrame()
    {
        return _frame_color;
    }
    
    void camera_base::change_white_balance(float red_level, float blue_level)
    {
        
    }
    
    void camera_base::change_gain(float gain_level)
    {
        
    }
    
    void camera_base::change_exposure_time(int time)
    {
        
    }
    
    void camera_base::change_black_level(float level)
    {
        
    }
    
    void camera_base::change_gamma(float gamma)
    {
        
    }
    
    void camera_base::frame_save(std::string frame_name, bool generate_file_seq)
    {
        std::string target_file = (!generate_file_seq) ? frame_name : filetools::generate_filename_seq(frame_name);
        
        mutex_lock();
            imwrite(target_file, _frame_color);
        mutex_unlock();
        
    }

    void camera_base::set_mutex_ptr(std::mutex* mutex_ptr)
    {
        _mutex_ptr = mutex_ptr;
    }
    
    std::mutex* camera_base::get_mutex_ptr()
    {
        return (_mutex_ptr != nullptr) ? _mutex_ptr : &_mutex_internal;
    }

    void camera_base::mutex_lock()
    {
        if (_mutex_ptr != nullptr)
            _mutex_ptr->lock();
        else
            _mutex_internal.lock();
    }
    
    void camera_base::mutex_unlock()
    {
        if (_mutex_ptr != nullptr)
            _mutex_ptr->unlock();
        else
            _mutex_internal.unlock();
    }    
    
    std::string camera_base::get_last_error()
    {
        return _last_error;
    }

}
