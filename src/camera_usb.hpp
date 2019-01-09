/*
 * Specific implementation for USB camera class 
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

#ifndef CAMERA_USB_HPP
#define CAMERA_USB_HPP

#include "camera_base.hpp"

namespace terraclear
{  
    class camera_usb : public camera_base
    {
        public:
            camera_usb(uint32_t camera_index);
            camera_usb(uint32_t camera_index, uint32_t resolutionX, uint32_t resolutionY);
            virtual ~camera_usb();
            
            //pure virtual implementation..
            void update_frames();
                        
        private:
            cv::VideoCapture _videofeed;

    };
}
#endif /* CAMERA_USB_HPP */

