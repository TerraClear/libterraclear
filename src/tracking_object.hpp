#ifndef TRACKING_OBJECT_HPP
#define TRACKING_OBJECT_HPP

#include <cstdlib>
#include <vector>
#include <list>
#include <math.h>

#include "detector_base.hpp"
#include "tracking_position.hpp"
#include "stopwatch.hpp"

namespace terraclear
{    
    class tracking_object
    {
        public: 
            tracking_object(bounding_box bbox, int history_depth);
            ~tracking_object();
                    
            int get_id();
            uint32_t get_position_count();
            
            float get_velocity_x();
            float get_velocity_y();
            float get_velocity_linear();
            
            void reset();
            void update(bounding_box bbox);

            bounding_box get_current();
            bounding_box get_predicted();

            //max amount of history to consider..
            int _max_history = 10;

        private:
            uint32_t _history_depth = 10;
            uint32_t _position_count = 0;
            stopwatch _sw;
            bounding_box _bbox;
            bounding_box _bbox_next;
            tracking_position _x_tracker;
            tracking_position _y_tracker;

            float _x_v = 0.0f;
            float _y_v = 0.0f;

            std::list<int> _last_widths;
            std::list<int> _last_heights;

            int get_list_median(std::list<int> value_list);
    };

}

#endif /* TRACKING_OBJECT_HPP */

