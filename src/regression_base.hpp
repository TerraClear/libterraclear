#ifndef REGRESSION_BASE_HPP
#define REGRESSION_BASE_HPP

#include <map>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <list>
#include "stopwatch.hpp"

namespace terraclear
{
    struct position_time
    {
        float time;
        int position;
    };
    struct regression_result
    {
        float slope;
        float intercept;
    };
            
    class regression_base
    {
        public:
            regression_base(int id, int queue_size, int starting_pos, float time_reset_thresh);
            void update_position(int pos);
            regression_result get_regression();
            
        protected:
            std::list<position_time> _position_history;
            float _time_sum;
            float _time_reset_thresh;
            int _queue_size;
            int _id;
            terraclear::stopwatch _sw;
    };
}

#endif
