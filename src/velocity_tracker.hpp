#ifndef VELOCITY_TRACKER_HPP
#define VELOCITY_TRACKER_HPP

#include <map>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include "regression_base.hpp"
#include "stopwatch.hpp"

namespace terraclear
{
    class velocity_tracker: public regression_base
    {
        public:
            velocity_tracker(terraclear::regression_obj_meta& info);
            void update_position(int pos);
            float get_velocity();
            void reset_anchor();
            
        private:
            int _previous_position;
            int _position_sum;
            int _anchor_pos;
            int _dist_reset_thresh;
    };
}

#endif
