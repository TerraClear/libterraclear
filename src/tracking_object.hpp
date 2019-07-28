#ifndef TRACKING_OBJECT_HPP
#define TRACKING_OBJECT_HPP

#include <cstdlib>
#include <vector>
#include <list>
#include <math.h>

#include "detector_base.hpp"
#include "regression_base.hpp"
#include "tracking_position.hpp"
#include "stopwatch.hpp"

namespace terraclear
{   
    class tracking_object
    {
        public: 
            tracking_object(regression_obj_meta& info);
            ~tracking_object();
                    
            int get_id();
            uint32_t get_position_count();
            
            float get_velocity_x();
            float get_velocity_y();
            float get_velocity_linear();
            
            void reset();
            void update(bounding_box bbox);
            void predict();
            void predict_average();

            bounding_box get_object();

            //max amount of history to consider..
            int _max_history = 20;
            
            float _frame_x_v = 0.0f;
            float _frame_y_v = 0.0f;

        private:
            uint32_t _history_depth = 10;
            uint32_t _position_count = 0;
            
            stopwatch _sw;
            bounding_box _bbox;
            terraclear::tracking_position _x_tracker;
            terraclear::tracking_position _y_tracker;

            float _x_v = 0.0f;
            float _y_v = 0.0f;
 
            std::list<int> _last_widths;
            std::list<int> _last_heights;

            int get_list_median(std::list<int> value_list);
    };

}

#endif /* TRACKING_OBJECT_HPP */

