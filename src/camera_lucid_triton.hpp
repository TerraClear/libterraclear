/*
 * Specific implementation for LUCID Triton camera class 
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


//only compile for Lucid Triton if required.. 
//i.e you MUST #define TC_USE_TRITON or use g++ with -DTC_USE_TRITON
#ifdef TC_USE_TRITON

#ifndef CAMERA_LUCID_TRITON_HPP
#define CAMERA_LUCID_TRITON_HPP

//Lucid Arena SDK.
#include "ArenaApi.h"

#include "camera_base.hpp"
#include "error_base.hpp"

// Width and height
#define LUCID_WIDTH 1440
#define LUCID_HEIGHT 1080

namespace terraclear
{ 
    typedef PfncFormat_ LUCID_PixelFormat;
    
    class camera_lucid_triton : public camera_base
    {
        public:
            camera_lucid_triton();
            virtual ~camera_lucid_triton();

            //pure virtual implementation..
            void update_frames();

        private:
            bool _disposing = false;
            
            const char* _base_errstr = "LUCID Triton Error - ";
            error_base get_generic_error(std::string);

            uint32_t                        _timeout_ms = 4000;
            LUCID_PixelFormat               _lucid_pixel_format = LUCID_PixelFormat::BayerRG8;
            Arena::ISystem*                 _lucid_system = nullptr;
            std::vector<Arena::DeviceInfo>  _lucid_camera_list;
            Arena::IDevice*                 _lucid_cam = nullptr;

            void init_lucid_system();
            uint32_t get_camera_count();
            void init_camera(uint32_t camera_index);
           
    };
}
#endif /* CAMERA_LUCID_TRITON_HPP */

#endif /* Conditional TRITON support */
