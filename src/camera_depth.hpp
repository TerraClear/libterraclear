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

#ifndef CAMERA_DEPTH_HPP
#define CAMERA_DEPTH_HPP

#include "camera_base.hpp"

#define INCHES_PER_CM 0.393701

namespace terraclear
{
    enum CameraPosition
    {
        Left,
        Right,
        Both
    };
    
    class camera_depth : public camera_base
    {
        
        public:            
            camera_depth();
            virtual ~camera_depth();

            //pure virtual function for acquiring depth (Z) and specific X Y  
            virtual double      get_depth_cm(uint32_t x, uint32_t y) = 0;
                    double      get_depth_inches(uint32_t x, uint32_t y);
            private:

    };    
}


#endif /* CAMERA_DEPTH_HPP */
