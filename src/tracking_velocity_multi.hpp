/*
 * helper class for tracking velocity of pixels in pixels per second 
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
 * CREATED BY: Isabelle Butterfield - isabelle.butterdield@terraclear.com
 * 
*/

#ifndef TRACKING_VELOCITY_MULTI_HPP
#define TRACKING_VELOCITY_MULTI_HPP

#include <map>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include "tracking_velocity.hpp"

namespace terraclear
{
    class velocity_calculator
    {
        public:
            velocity_calculator();
            void add_tracker(int id, int initial_pos);
            void remove_tracker(int id);
            void update_tracker_position(int id, int current_pos);
            void reset_tracker_anchor(int id);
            float get_average_velocity();
        private:
            std::map<int,terraclear::tracking_velocity*> _trackers;
    };
}

#endif //TRACKING_VELOCITY_MULTI_HPP
