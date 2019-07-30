#ifndef VELOCITY_CALCULATOR_HPP
#define VELOCITY_CALCULATOR_HPP

#include <map>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include "tracking_position.hpp"

namespace terraclear
{
    class velocity_calculator
    {
        public:
            velocity_calculator(int start_fps, float time_reset_thresh, int dist_reset_thresh);
            void add_tracker(int id, int queue_size, int starting_ypos, int max_travel_dist);
            void update_tracker_position(int id, int ypos);
            void reset_tracker_anchor(int id, int pos);
            float get_average_velocity();
        private:
            std::map<int,terraclear::tracking_position*> _trackers;
            int _starting_fps;
            float _time_reset_thresh;
            int _dist_reset_thresh;
    };
}

#endif
