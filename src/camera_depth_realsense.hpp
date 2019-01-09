/*
 * Specific implementation for RealSense 3D camera class 
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

//only compile for RealSense if required.. 
//i.e you MUST #define TC_USE_REALSENSE or use g++ with -DTC_USE_REALSENSE

#ifdef TC_USE_REALSENSE

#include <cstdlib>
#include <iostream>

#include "camera_depth.hpp"
#include <librealsense2/rs.hpp>

#ifndef CAMERA_DEPTH_REALSENSE_HPP
#define CAMERA_KDEPTH_REALSENSE_HPP

namespace terraclear
{
    class camera_depth_realsense : public camera_depth
    {

        public:
            camera_depth_realsense();
            virtual ~camera_depth_realsense();

            //base class implementations.. 
            double      get_depth_cm(uint32_t x, uint32_t y);
            double      get_depth_center_cm();
            void        update_frames();
            
            int         avgerage_square_pixels = 5;
        
    private:
            rs2::pipeline   _pipe;
            rs2::config     _pipe_config;
            std::shared_ptr<rs2::depth_frame> _rls_frame_depth;
            
            double get_depth_internal(uint32_t x, uint32_t y);
            
    };
}

#endif /* CAMERA_DEPTH_REALSENSE_HPP */

#endif
