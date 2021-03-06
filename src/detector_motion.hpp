/*
 * basic motion detector
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

#ifndef DETECTOR_MOTION_HPP
#define DETECTOR_MOTION_HPP

#include "detector_base.hpp"

namespace terraclear
{
    class detector_motion : public detector_base
    {
        public:
            detector_motion(cv::Mat imgsrc);
            virtual ~detector_motion();

            //pixel area as min area to change to trigger motion.
            void set_motion_threshold(double motion_threshold);
            void set_blur_kernel(unsigned short kernel_size);
            
            //pure virtual implementation
            std::vector<bounding_box> detect_objects();

            //debug mode
            bool _debug_mode = false;
            bool _contour_mode = false;
            
        private:
            double _motion_threshold = 150;
            double _kernel_size = 5;
            cv::Mat _imgsrc_old;
            uint64_t _detect_count = 0;

    };
}
#endif /* DETECTOR_MOTION_HPP */

