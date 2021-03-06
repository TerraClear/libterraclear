/*
 * Specific implementation for USB camera class 
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

#include "camera_usb.hpp"

namespace terraclear
{

    camera_usb::camera_usb(uint32_t camera_index) 
    {
        if(!_videofeed.open(camera_index))
        {
            //TODO: Add custom exception handlers..
            std::cerr << "\n** Camera Not Available!!\n";
            
            throw std::exception();
        }
    }
    
    camera_usb::camera_usb(uint32_t camera_index, uint32_t resolutionX, uint32_t resolutionY)
        : camera_usb(camera_index)
    {
        //Set Camera Resolution
        _videofeed.set(3,resolutionX);
        _videofeed.set(4,resolutionY);
    }
    

    camera_usb::~camera_usb() 
    {
        _videofeed.release();
    }

    bool camera_usb::frame_update()
    {
        mutex_lock();
        _videofeed.read(_frame_color);
        mutex_unlock();
        
        return true;
    }
}