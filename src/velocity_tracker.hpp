#ifndef VELOCITY_TRACKER_HPP
#define VELOCITY_TRACKER_HPP

#include <map>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include "stopwatch.hpp"

namespace terraclear
{
    class velocity_tracker
    {
        public:
            velocity_tracker(int id, int starting_ypos);
            int get_id();
            void update_position(int ypos);
            float get_velocity();
            void reset_anchor();
        private:
            std::map<int,int> _frame_locations;
            int _tracker_id;
            float _fps_sum;
            int _frame_count;
            int _current_frame_number;
            terraclear::stopwatch _sw;
    };
}

#endif
