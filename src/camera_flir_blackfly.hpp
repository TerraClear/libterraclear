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


//only compile for FLIR Blackfly S if required.. 
//i.e you MUST #define TC_USE_BLACKFLY or use g++ with -DTC_USE_BLACKFLY
//#ifdef TC_USE_BLACKFLY

#ifndef CAMERA_FLIR_BLACKFLY_HPP
#define CAMERA_FLIR_BLACKFLY_HPP

// Width and height
#define FLIR_WIDTH 1440
#define FLIR_HEIGHT 1080

#define FLIR_ERR_STR "FLIR BlackFlyS Error - "

#include "Spinnaker.h"
#include "SpinGenApi/SpinnakerGenApi.h"

#include "camera_base.hpp"
#include "error_base.hpp"

namespace flir = Spinnaker;
namespace flir_api = Spinnaker::GenApi;
namespace flir_icam = Spinnaker::GenICam;

namespace terraclear
{ 
    typedef flir::PixelFormatEnums FLIR_PixelFormat;
    
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
                float   exposure_time;
            };

            camera_flir_blackfly(flir_settings cam_settings);
            camera_flir_blackfly(flir_settings cam_settings, uint32_t cam_index);
            camera_flir_blackfly(flir_settings cam_settings, std::string cam_serial);
            virtual ~camera_flir_blackfly();

            //pure virtual implementation..
            void update_frames();
            
            //get camera list
            static std::vector<std::string> get_cameras();

        private:
            static error_base get_flir_error(flir::Exception &e);
            static error_base get_generic_error(std::string);

            bool _disposing = false;
            flir_settings _cam_settings;
            
            flir::SystemPtr _flir_system = nullptr;
            flir::CameraList _flir_camera_list;
            flir::CameraPtr _flir_cam = nullptr;

            void init_flir_system();
            uint32_t get_camera_count();
            void init_camera();
            const char* flir_pixel_format_to_string(FLIR_PixelFormat flir_pixel_format);
            

    };
}
#endif /* CAMERA_FLIR_BLACKFLY_HPP */

//#endif /* Conditional BLACKFLY support */
