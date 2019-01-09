/*
 * Specific implementation for Video from File class 
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

#include "camera_file.hpp"

namespace terraclear
{  

    camera_file::camera_file(std::string file_path)
    {
        if(!_videofeed.open(file_path))
        {
            //TODO: Add custom exception handlers..
            throw std::exception();
        }        
    }

    camera_file::~camera_file() 
    {
        _videofeed.release();
    }
    
    void camera_file::update_frames()
    {
        //get next frame;
        _videofeed.read(_frame_color);
    }
    
}