/*
 * Specific implementation for Video from File class 
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

#ifndef CAMERA_FILE_HPP
#define CAMERA_FILE_HPP

#include <iostream>
#include "camera_base.hpp"

namespace terraclear
{  

    class camera_file : public camera_base
    {
        public:
            camera_file(std::string file_path);
            virtual ~camera_file();
           
            //pure virtual implementation..
            bool frame_update();
                        
        private:
            cv::VideoCapture _videofeed;
    };
}
#endif /* CAMERA_FILE_HPP */

