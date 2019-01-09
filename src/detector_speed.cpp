/*
 * helper class for calculating average speed
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

#include "detector_speed.hpp"

namespace terraclear
{
    detector_speed::detector_speed()
    {        
    }

    detector_speed::detector_speed(uint16_t max_measures, uint64_t reset_timeout_ms, float max_error) 
    {
        _max_measures = max_measures;
        _reset_timeout_ms = reset_timeout_ms;
        _max_error = max_error;
        
        _last_time_check = std::chrono::steady_clock::now();
    }

    detector_speed::~detector_speed() 
    {
    }
    
    void detector_speed::set_max_measures(uint16_t max_measures)
    {
        _max_measures = max_measures;
    }
    
    void detector_speed::set_reset_timeout(uint64_t reset_timeout_ms)
    {
        _reset_timeout_ms = reset_timeout_ms;
    }
    
    void detector_speed::set_max_error(float max_error)
    {
        _max_error = max_error;
    }
    
    bool detector_speed::update_speed(uint32_t item_id, float item_position)
    {
        //un-tracked = 0
        if (item_id == 0)
            return false;
        
        bool retval = false;
        std::chrono::steady_clock::time_point time_update =  std::chrono::steady_clock::now();
         
        //check if this item is already being measured?, if so, just add a measurement
        float last_position = 0.0f;
        int item_count = _items_positions.count(item_id);
        int pos_count = 0;
        if (item_count <= 0)
        {
            //not in list yet, add first one.
            std::list<time_position> tmp_lst;
            _items_positions[item_id] = tmp_lst;
        }
        else 
        {
            //get last position
            pos_count =_items_positions[item_id].size();
            last_position =  (pos_count > 0) ? _items_positions[item_id].front().position_check : 0;

            //pop and re-push all entires, skipping any time outs.
            for (int i = 0; i < pos_count; i++)
            {
                time_position p_item = _items_positions[item_id].back();
                _items_positions[item_id].pop_back();

                //check that item has not expired, re-add if alive or delete if expired.
                if (std::chrono::duration_cast<std::chrono::milliseconds>(time_update - p_item.time_check).count() < _reset_timeout_ms)
                    _items_positions[item_id].push_front(p_item);


            }        
        }


        //make sure measurement is within error bounds..
        pos_count =_items_positions[item_id].size();
        if (abs(item_position - last_position) > _max_error)
        {
            // trim excess measurements..
            if (pos_count > _max_measures)
                _items_positions[item_id].pop_back();
            
            //add new  measurement
            time_position p_timepos;
            p_timepos.position_check = item_position;
            p_timepos.time_check = time_update;
            _items_positions[item_id].push_front(p_timepos);

            retval = true;
        }
        else if (pos_count > 0)
        {
            //just update last time_check
            _items_positions[item_id].front().time_check = time_update;
        }
        
        return retval;
    }
    
    uint16_t detector_speed::get_queue_size(uint32_t item_id)
    {
        uint16_t retval = 0;

        //if item exists.. get queue size
        if (_items_positions.count(item_id))
        {
            //get queue size
            retval =  _items_positions[item_id].size();
        }
        
        return retval;
    }

    float detector_speed::get_speed_ups(uint32_t item_id)
    {
        float retval = 0.0f;
        
        //if item exists.. calculate avg speed for all measurements.
        if (_items_positions.count(item_id))
        {
            if (_items_positions[item_id].size() > 1)
            {
                //start and min positions and time windows
                float position_start = std::numeric_limits<float>::max();
                float position_end = std::numeric_limits<float>::min();
                std::chrono::steady_clock::time_point time_end = std::chrono::steady_clock::time_point::min();
                std::chrono::steady_clock::time_point time_start = std::chrono::steady_clock::time_point::max();

                for (auto item_checkpoint : _items_positions[item_id])
                {
                    //get start and end time & positions
                    position_start = std::min(position_start, item_checkpoint.position_check);
                    position_end = std::max(position_end, item_checkpoint.position_check);
                    time_start = std::min(time_start, item_checkpoint.time_check);
                    time_end = std::max(time_end, item_checkpoint.time_check);
                    
                }

                //total duration_ms & distance
                float duration_s = (float)std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start).count() / 1000;     
                float displacement = position_end - position_start;
                
                //calc speed
                retval = displacement / duration_s;
            }
          
        }
        
        return retval;
    }

}