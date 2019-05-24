#ifndef ROCK_DISTANCE_TRACKER_HPP
#define ROCK_DISTANCE_TRACKER_HPP

#include <map>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <deque>     
#include "stopwatch.hpp"

namespace terraclear
{
    class rock_distance_tracker
    {
        public:
            rock_distance_tracker(int starting_ypos, int dest_ypos, int queue_size);
            void update_position(int ypos);
            float get_distance();
        private:
            std::deque<int> _distances;
            int _queue_size;
            float _fps_sum;
            int _dest_ypos;
    };
}

#endif

