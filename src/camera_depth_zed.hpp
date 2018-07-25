/*
 * Specific implementation for StereoLabs ZED 3D camera class 
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

#ifndef CAMERA_DEPTH_ZED_HPP
#define CAMERA_DEPTH_ZED_HPP

#include "camera_depth.hpp"

#define ZED_RESX 1920 * 2
#define ZED_RESY 1080

namespace terraclear
{
    class camera_depth_zed : public camera_depth
    {
     
        public:
            camera_depth_zed(uint32_t camera_index, CameraPosition selected_cam);
            virtual ~camera_depth_zed();

            //base class implementations.. 
            double      get_depth_cm(uint32_t x, uint32_t y);
            void        update_frames();

        private:
            CameraPosition _selected_camera = CameraPosition::Both;
            cv::VideoCapture _videofeed;
            
    };
}
#endif /* CAMERA_DEPTH_ZED_HPP */

