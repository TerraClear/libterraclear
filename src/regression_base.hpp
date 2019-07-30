#ifndef REGRESSION_BASE_HPP
#define REGRESSION_BASE_HPP

#include <map>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <list>
#include "stopwatch.hpp"
#include "detector_base.hpp"

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

    struct regression_obj_meta
    {
        terraclear::bounding_box bbox;
        int bbox_id;
        int queue_size;
        int starting_pos;
        int dest_pos;
        float time_reset_thresh;
        int dist_reset_thresh;
    };
    
    class regression_base
    {
        public:
            regression_base(terraclear::regression_obj_meta& info);
            void update_position(int& pos);
            regression_result get_regression() const;
            
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
