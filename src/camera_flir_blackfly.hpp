/*
 * Specific implementation for FLIR Blackfly S camera class 
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

#define TC_USE_BLACKFLY

//only compile for FLIR Blackfly S if required.. 
//i.e you MUST #define TC_USE_BLACKFLY or use g++ with -DTC_USE_BLACKFLY
#ifdef TC_USE_BLACKFLY

#ifndef CAMERA_FLIR_BLACKFLY_HPP
#define CAMERA_FLIR_BLACKFLY_HPP

// Width and height
#define FLIR_WIDTH 1440
#define FLIR_HEIGHT 1080

#define FLIR_DEVICE_LINK_LIMIT 114825323

#define FLIR_PIXEL_FORMAT Spinnaker::PixelFormatEnums::PixelFormat_BayerRG8

#include "Spinnaker.h"
#include "SpinGenApi/SpinnakerGenApi.h"

#include "camera_flir_blackfly_system.hpp"
#include "camera_base.hpp"
#include "error_base.hpp"

namespace flir = Spinnaker;
namespace flir_api = Spinnaker::GenApi;
namespace flir_icam = Spinnaker::GenICam;

namespace terraclear
{ 
    typedef flir::PixelFormatEnums FLIR_PixelFormat;
    typedef flir::ImageStatus FLIR_ImageStatus;
    
    class camera_flir_blackfly : public camera_base
    {
        public:
            struct flir_settings
            {
                FLIR_PixelFormat pixel_format;
                int     width;
                int     height;
                int     bin_vertical;
                int     bin_horizontal;
                bool    flip_y;
                float   fps;
                bool    exposure_auto;
                float   exposure_time;
                int     device_link_limit;
            };

            camera_flir_blackfly(camera_flir_blackfly_system* flir_system_ptr, flir_settings cam_settings);
            camera_flir_blackfly(camera_flir_blackfly_system* flir_system_ptr, flir_settings cam_settings, uint32_t cam_index);
            camera_flir_blackfly(camera_flir_blackfly_system* flir_system_ptr, flir_settings cam_settings, std::string cam_serial);
            virtual ~camera_flir_blackfly();

            std::string get_serial();

            //pure virtual implementation..
            bool frame_update();
            
            void release();
        private:
            cv::Mat _buffer_camera;

            bool _disposing = false;
            flir_settings _cam_settings;
  
            camera_flir_blackfly_system* _flir_system_ptr = nullptr;
            flir::CameraPtr _flir_cam = nullptr;
            std::string _cam_serial;

            void init_camera();
            std::string flir_pixel_format_to_string(FLIR_PixelFormat flir_pixel_format);
            std::string flir_imgstatus_to_string(FLIR_ImageStatus flir_image_status);
            
    };
}
#endif /* CAMERA_FLIR_BLACKFLY_HPP */

#endif /* Conditional BLACKFLY support */