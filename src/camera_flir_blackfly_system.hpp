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

//#define TC_USE_BLACKFLY

//only compile for FLIR Blackfly S if required.. 
//i.e you MUST #define TC_USE_BLACKFLY or use g++ with -DTC_USE_BLACKFLY
#ifdef TC_USE_BLACKFLY

#ifndef CAMERA_FLIR_BLACKFLY_SYSTEM_HPP
#define CAMERA_FLIR_BLACKFLY_SYSTEM_HPP

#include <iostream>
#include <mutex>

#include "Spinnaker.h"
#include "SpinGenApi/SpinnakerGenApi.h"

#include "error_base.hpp"

#define FLIR_ERR_STR "FLIR BlackFlyS Error - "

namespace flir = Spinnaker;
namespace flir_api = Spinnaker::GenApi;
namespace flir_icam = Spinnaker::GenICam;

namespace terraclear
{
    class camera_flir_blackfly_system 
    {
        public:
            camera_flir_blackfly_system();
            virtual ~camera_flir_blackfly_system();

            //get camera list
            std::vector<std::string> get_cameras();
            uint32_t get_camera_count();
            
            flir::CameraPtr get_camera(std::string cam_serial);
            flir::CameraPtr get_camera(int cam_index);

            static error_base get_flir_error(flir::Exception &e);
            static error_base get_generic_error(std::string);

            void release();
        private:
            bool _disposing = false;
            flir::SystemPtr _flir_system = nullptr;
            flir::CameraList _flir_camera_list;
            std::mutex _mutex;

    };
    
}

#endif /* CAMERA_FLIR_BLACKFLY_SYSTEM_HPP */

#endif /* Conditional BLACKFLY support */
