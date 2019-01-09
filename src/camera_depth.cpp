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
         // given height above ground and distance to ground on camera plane 
        // at center of image, calculate ground angle in radians
        return std::asin(camera_height_cm / get_depth_center_cm());
    }

    double camera_depth::get_distance_over_ground_cm(cv::Point a, cv::Point b, double camera_angle_rad)
    {
        //calculate point a distance from origin ( i.e. where camera plane and ground plane intersect)
        double a_depth = get_depth_cm(a.x, a.y); 
        // distance over ground from origin to point a
        double origin_to_a = a_depth / std::cos(camera_angle_rad); 

        //calculate point b distance from origin (i.e. where camera plane and ground plane intersect)
        double b_depth = get_depth_cm(b.x, b.y);
        // distance over ground from origin.to point b
        double origin_to_b = b_depth / std::cos(camera_angle_rad); 

        //distance between a & b in centimeters..
        double retval = origin_to_b - origin_to_a;
        
        return retval;
    }
}
