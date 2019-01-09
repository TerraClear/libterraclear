/*
 * Basic Stopwatch Timer
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
*/

#ifndef STOPWATCH_HPP
#define STOPWATCH_HPP

#include <vector>
#include <chrono>
#include <mutex>

namespace  terraclear
{
    class stopwatch 
    {
        public:
            struct laptime
            {
                uint64_t elapsed_time;
                uint64_t lap_time;
            };

            stopwatch();
            virtual ~stopwatch();
            
            void start();
            void stop();
            void reset();
            uint32_t lap();
            
            uint64_t get_elapsed_us();
            uint64_t get_elapsed_ms();
            uint64_t get_elapsed_s();

            laptime get_lap_us(uint32_t lap_id);
            laptime get_lap_ms(uint32_t lap_id);
            laptime get_lap_s(uint32_t lap_id);
            
        private:
            std::chrono::steady_clock::time_point _lastmeasure;              
            uint64_t _elapsed_us = 0;
            uint64_t _prev_elapsed_us = 0;
            std::vector<laptime> _lap_times;
            bool _isrunning = false;
            std::mutex _mutex;
            
            void update_clock();

    };
}
#endif /* STOPWATCH_HPP */

