#ifndef ROCK_DISTANCE_TRACKER_HPP
#define ROCK_DISTANCE_TRACKER_HPP

#include <map>
#include <unistd.h>
#include <stdio.h>
#include <iostream>   
#include "regression_base.hpp"
#include "stopwatch.hpp"

namespace terraclear
{
    class rock_distance_tracker: public regression_base
    {
        public:
            rock_distance_tracker(int id, int queue_size, int starting_pos, int dest_pos, float time_reset_thresh);
            void update_position(int pos);
            float get_distance();
        private:
            int _dest_pos;
    };
}

#endif

