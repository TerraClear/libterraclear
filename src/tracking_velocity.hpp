/*
 * helper class for calculating average velocity using linear regression
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

#ifndef TRACKING_VELOCITY_HPP
#define TRACKING_VELOCITY_HPP

#include <map>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include "stopwatch.hpp"

namespace terraclear
{
    class tracking_velocity
    {
        public:
            tracking_velocity(int id, int starting_pos);

            int get_id();
            void update_position(int current_pos);
            float get_velocity();
            void reset_position();

    private:
            std::map<int,int> _position_history;
            int _tracker_id;
            float _positions_per_sec_sum;
            uint32_t _position_count;
            int _last_position_index;
            terraclear::stopwatch _sw;
    };
}

#endif //TRACKING_VELOCITY_HPP
