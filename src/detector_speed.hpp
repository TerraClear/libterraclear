#include <list>
#include <map>
#include <chrono>
#include <opencv2/core/hal/interface.h>

#ifndef DETECTOR_SPEED_HPP
#define DETECTOR_SPEED_HPP

namespace terraclear
{

    class detector_speed
    {
        public:
            detector_speed();
            detector_speed(uint16_t max_measures, uint64_t reset_timeout_ms, float max_error);
            virtual ~detector_speed();

            //set defaults
            void set_max_measures(uint16_t max_measures);
            void set_reset_timeout(uint64_t reset_timeout_ms);
            void set_max_error(float max_error);

            //update internal list of tracked objects
            bool update_speed(uint32_t item_id, float item_position);
            
            //get queue depth of specific object
            uint16_t get_queue_size(uint32_t item_id);
            
            //get speed units per second.
            float get_speed_ups(uint32_t item_id);
            
        private:
            // max error to ignore and not update speed..
            float _max_error = 1.0f; 
            
            // max amount of measurements to average across
            uint16_t _max_measures = 10;
            
            // milliseconds to assume measurement is stale and remove from map..
            uint64_t _reset_timeout_ms = 1000;
            
            // last timeout check.
            std::chrono::steady_clock::time_point _last_time_check;
             
            //struct to hold each time/distance measure
            struct time_position
            {
                std::chrono::steady_clock::time_point time_check;
                float position_check;
            };

            //dictionary to hold distances mesaures per unique_item
            std::map<uint32_t, std::list<time_position>> _items_positions;
    };
}
#endif /* DETECTOR_SPEED_HPP */

