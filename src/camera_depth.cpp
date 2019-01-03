/*
 * Base Depth Camera class 
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


#include "camera_depth.hpp"
namespace terraclear
{
    camera_depth::camera_depth() 
    {
    }

    camera_depth::~camera_depth() 
    {
    }
    
    cv::Mat camera_depth::getDepthFrame()
    {
        return _depth_frame;
    }
    
    double camera_depth::get_angle_center_rad(double camera_height_cm)
    {
        return std::asin(camera_height_cm / get_depth_center_cm()); //calculate ground angle in radians
    }

}
