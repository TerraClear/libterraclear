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
    
   void camera_base::set_mutex_ptr(std::mutex* mutex_ptr)
   {
       _mutex_ptr = mutex_ptr;
   }
    
    std::mutex* camera_base::get_mutex_ptr()
    {
        return _mutex_ptr;
    }

    bool camera_base::mutex_lock()
    {
        bool success = false;
        
        if (_mutex_ptr != nullptr)
        {
            _mutex_ptr->lock();
            success = true;
        }
       
        return success;
    }
    bool camera_base::mutex_unlock()
    {
        bool success = false;
        
        if (_mutex_ptr != nullptr)
        {
            _mutex_ptr->unlock();
            success = true;
        }
       
        return success;
    }    
    
    std::string camera_base::get_last_error()
    {
        return _last_error;
    }

}
