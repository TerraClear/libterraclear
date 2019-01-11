/*
 * helper class for calculating average of measurements over time
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
#include <list>
#include <map>
#include <chrono>
#include <algorithm>

#include <opencv2/core/hal/interface.h>

#ifndef TIMED_AVERAGING_HPP
#define TIMED_AVERAGING_HPP

namespace terraclear
{

    class timed_averaging
    {
        public:
            timed_averaging();
            timed_averaging(uint16_t max_measures, uint64_t reset_timeout_ms, float max_error);
            virtual ~timed_averaging();

            //set defaults
            void set_max_measures(uint16_t max_measures);
            void set_reset_timeout(uint64_t reset_timeout_ms);
            void set_max_error(float max_error);

            //update internal list of tracked objects
            bool update_measurement(uint32_t item_id, float measurement_value);
            
            //get queue depth of specific object
            uint16_t get_queue_size(uint32_t item_id);
            
            //get rate of change in units per second.
            float get_rate_of_change_sec(uint32_t item_id);

            //get average for specific item
            float get_item_average(uint32_t item_id);

            //get average for specific item
            float get_item_median(uint32_t item_id);
            
            //sum over time
            double get_item_sum(uint32_t item_id);

            
        private:
            //struct to hold each time/distance measure
            struct time_position
            {
                std::chrono::steady_clock::time_point measurement_time;
                float measurement_value;
            };

            // max error to ignore and not update speed..
            float _max_error = 0.0f; 
            
            // max amount of measurements to average across
            uint16_t _max_measures = 10;
            
            // milliseconds to assume measurement is stale and remove from map..
            uint64_t _reset_timeout_ms = 1000;
            
            // last timeout check.
            std::chrono::steady_clock::time_point _last_time_measured;
             
            //dictionary to hold distances mesaures per unique_item
            std::map<uint32_t, std::list<time_position>> _items_measurements;
    };
}
#endif /* TIMED_AVERAGING_HPP */

