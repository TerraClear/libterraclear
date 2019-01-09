/*
 * Base C++ cross platform camera class 
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

#ifndef CAMERA_BASE_HPP
#define CAMERA_BASE_HPP

#include <opencv2/opencv.hpp>

namespace terraclear
{    
    class camera_base 
    {
        public:
            camera_base();
            virtual ~camera_base();

            virtual void    update_frames() = 0;
            cv::Mat         getRGBFrame();

        protected:
            cv::Mat _frame_color;
            
        private:

    };
}
#endif /* CAMERA_BASE_HPP */

