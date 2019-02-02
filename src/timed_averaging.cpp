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

#include <vector>

#include "timed_averaging.hpp"

namespace terraclear
{
    timed_averaging::timed_averaging()
    {        
    }

    timed_averaging::timed_averaging(uint16_t max_measures, uint64_t reset_timeout_ms, float max_error) 
    {
        _max_measures = max_measures;
        _reset_timeout_ms = reset_timeout_ms;
        _max_error = max_error;
    }

    timed_averaging::~timed_averaging() 
    {
    }
    
    void timed_averaging::set_max_measures(uint16_t max_measures)
    {
        _max_measures = max_measures;
    }
    
    void timed_averaging::set_reset_timeout(uint64_t reset_timeout_ms)
    {
        _reset_timeout_ms = reset_timeout_ms;
    }
    
    void timed_averaging::set_max_error(float max_error)
    {
        _max_error = max_error;
    }
    
    bool timed_averaging::update_measurement(uint32_t item_id, float measurement_value)
    {
        //un-tracked = 0
        if (item_id == 0)
            return false;
        
        bool retval = false;
        
        //get current time
        std::chrono::steady_clock::time_point time_update =  std::chrono::steady_clock::now();

        //intialize  previous time and value for this item
        std::chrono::steady_clock::time_point last_update = time_update;
        float last_measurement = measurement_value;

        //update to the last stored value/time if any is available.
        if (_item_previous_measurement.count(item_id) > 0)
        {
           last_update = _item_previous_measurement[item_id].measurement_time;
           last_measurement = _item_previous_measurement[item_id].measurement_value;
        }
        
        //check if any measurements have been made, if not add list
        int item_count = _item_timed_measurements.count(item_id);
        if (item_count <= 0)
        {
            //not in list yet, add first one.
            std::list<timed_measurement> tmp_lst;
            _item_timed_measurements[item_id] = tmp_lst;
        }
        else 
        {
            //get count of measurements and remove any stale
           int pos_count =_item_timed_measurements[item_id].size();

            //pop and re-push all entries, skipping any time outs.
            for (int i = 0; i < pos_count; i++)
            {
                timed_measurement p_item = _item_timed_measurements[item_id].back();
                _item_timed_measurements[item_id].pop_back();

                //check that item has not expired, re-add if alive or delete if expired.
                uint64_t time_lapased = std::chrono::duration_cast<std::chrono::milliseconds>(time_update - p_item.measurement.measurement_time).count();
                if (time_lapased < _reset_timeout_ms)
                    _item_timed_measurements[item_id].push_front(p_item);
            }  

        }            

        //make sure measurement is within error bounds OR its the first measurement..
        if ((abs(last_measurement - measurement_value) > _max_error) || (_item_previous_measurement.count(item_id) == 0))
        {
            // trim excess measurements..
            if (_item_timed_measurements[item_id].size() > _max_measures)
                _item_timed_measurements[item_id].pop_back();

            //create measurement new entry
            timed_measurement tmp_shft;
            tmp_shft.elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_update - last_update).count();
            tmp_shft.change_units = last_measurement - measurement_value;
            tmp_shft.measurement.measurement_time = time_update;
            tmp_shft.measurement.measurement_value = measurement_value;

            // add measurement
            _item_timed_measurements[item_id].push_front(tmp_shft);

            //update last measurement for this item_id
            _item_previous_measurement[item_id] =  tmp_shft.measurement;

            retval = true;
        }

        return retval;
    }
    
    uint16_t timed_averaging::get_queue_size(uint32_t item_id)
    {
        uint16_t retval = 0;

        //if item exists.. get queue size
        if (_item_timed_measurements.count(item_id) > 0)
        {
            //get queue size
            retval =  _item_timed_measurements[item_id].size();
        }
        
        return retval;
    }

    float timed_averaging::get_avg_rate_of_change(uint32_t item_id)
    {
        float retval = 0.0f;

        //if item exists.. get avg integrated rate of change for all measurements.
        if (_item_timed_measurements.count(item_id) > 0)
        {
            float dislacement = 0.00f;
            uint64_t ms_elapsed = 0;
            for (auto item_measured : _item_timed_measurements[item_id])
            {
                dislacement += item_measured.change_units;
                ms_elapsed += item_measured.elapsed_ms;
            }
            
            retval = 1000 / ((float)ms_elapsed / dislacement);
        }
        
        return retval;
    }

    double timed_averaging::get_item_sum(uint32_t item_id)
    {
        double retval = 0.0f;

        //if item exists.. calculate sum of all measurements.
        if (_item_timed_measurements.count(item_id) > 0)
        {
            if (_item_timed_measurements[item_id].size() > 0)
            {
                for (auto item_checkpoint : _item_timed_measurements[item_id])
                {
                    retval += item_checkpoint.measurement.measurement_value;
                }
            }
        }
        
        return retval;
    }

    float timed_averaging::get_item_average(uint32_t item_id)
    {
        float retval = 0.0f;
        double item_sum = get_item_sum(item_id);

        retval = (item_sum > 0) ? item_sum / _item_timed_measurements[item_id].size() : 0 ;

        return retval;
    }

    float  timed_averaging::get_item_median(uint32_t item_id)
    {
        float retval = 0.0f;

        //if item exists.. get median measurement for all measurements.
        if (_item_timed_measurements.count(item_id) > 0)
        {
            if (_item_timed_measurements[item_id].size() > 2) //need at least 3 for median..
            {
                std::vector<float> measures;
                for (auto item_checkpoint : _item_timed_measurements[item_id])
                    measures.push_back(item_checkpoint.measurement.measurement_value);
                
                //median
                 std::nth_element(measures.begin(), measures.begin() + measures.size()/2, measures.end());
                 retval = measures[_item_timed_measurements[item_id].size()/2];
            }
        }
        
        return retval;
    }
}