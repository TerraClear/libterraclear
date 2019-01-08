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
        bool retval = false;
        //timeout check and remove any stale measurements from _items_positions       
        //TODO - KOOS: Check front of queues for timeouts and remove..
        
        //check if this item is already being measured?, if so, just add a measurement
        float last_position = 0.0f;
        int item_count = _items_positions.count(item_id);
        if (item_count <= 0)
        {
            //not in list yet, add first one.
            std::list<time_position> tmp_lst;
            _items_positions[item_id] = tmp_lst;
        }
        else 
        {
            //get last position
            last_position =  _items_positions.at(item_id).front().position_check;
        }

        //make sure measurement is within error bounds..
        if (abs(item_position - last_position) > _max_error)
        {
            // trim excess measurements..
            if (_items_positions.at(item_id).size() >= _max_measures)
                _items_positions.at(item_id).pop_back();

            //add new  measurement
            time_position obj_dt;
            obj_dt.position_check = item_position;
            obj_dt.time_check = std::chrono::steady_clock::now();
            _items_positions.at(item_id).push_front(obj_dt);

            retval = true;
        }
        else
        {
            //just update time_check
            _items_positions.at(item_id).front().time_check = std::chrono::steady_clock::now();
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
            retval =  _items_positions.at(item_id).size();
        }
        
        return retval;
    }

    float detector_speed::get_speed_ups(uint32_t item_id)
    {
        float retval = 0.0f;
        
        //TODO: KOOS - calculate speed of position/time measurements
        
        return retval;
    }

}