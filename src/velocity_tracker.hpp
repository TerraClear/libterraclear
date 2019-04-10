#ifndef VELOCITY_TRACKER_HPP
#define VELOCITY_TRACKER_HPP

#include <map>
#include <unistd.h>
#include <stdio.h>
#include <iostream>

namespace terraclear
{
    class velocity_tracker
    {
        public:
            velocity_tracker(int fps, int id);
            int get_id();
            void update_position(int ypos);
            float find_velocity();
            void reset_anchor();
        private:
            std::map<int,int> _frame_locations;
            int _tracker_id;
            int _fps;
            int _current_frame_number;
    };
}

#endif
