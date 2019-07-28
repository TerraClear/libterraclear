#ifndef TRACKING_POSITION_HPP
#define TRACKING_POSITION_HPP

#include <map>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <deque>     

#include "regression_base.hpp"
#include "stopwatch.hpp"

namespace terraclear
{
    class tracking_position: public regression_base
    {
        public:
            struct tracking_info
            {
                float position;
                float velocity;
            };

            tracking_position(regression_obj_meta& info);
            void update_rock_pos(int new_position);
            tracking_info get_tracking_info();
            
            void update_position(int pos);
            float get_velocity();
            void reset_anchor();
            
        private:
            stopwatch _sw;
            int _queue_size;
            std::deque<int> _positions;
            std::deque<uint64_t> _intervals_ms;

            float _stable_position = 0.0f;
            float _pixels_per_second = 0.0f; 
            
            int _previous_position;
            int _position_sum;
            int _anchor_pos;
            int _dist_reset_thresh;
    };
}

#endif //TRACKING_POSITION_HPP

