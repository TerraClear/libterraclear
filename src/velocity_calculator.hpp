#ifndef VELOCITY_CALCULATOR_HPP
#define VELOCITY_CALCULATOR_HPP

#include <map>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include "velocity_tracker.hpp"

namespace terraclear
{
    class velocity_calculator
    {
        public:
            velocity_calculator(int start_fps);
            void add_tracker(int id, int starting_ypos);
            void update_tracker_position(int id, int ypos);
            void reset_tracker_anchor(int id);
            float get_average_velocity();
        private:
            std::map<int,terraclear::velocity_tracker*> _trackers;
            int _starting_fps;
    };
}

#endif
