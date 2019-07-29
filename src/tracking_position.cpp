#include "regression_base.hpp"
#include "tracking_position.hpp"
#include "math.h"

namespace terraclear
{
	tracking_position::tracking_position(terraclear::regression_obj_meta& info): regression_base(info)
	{   
            _queue_size = info.queue_size;
            _sw.start();
            _sw.reset();
            
            _previous_position = info.starting_pos;
            _position_sum = 0.f;
            _anchor_pos = info.starting_pos;
            _dist_reset_thresh = info.dist_reset_thresh;
	}

	void tracking_position::update_rock_pos(int pos)
	{       
            regression_base::update_position(pos);
	}
        
        void tracking_position::update_position(int pos)
	{
            //If distance traveled getting too large, set values back
            if (_position_history.front().position > _dist_reset_thresh)
            {   
                typedef std::list< position_time > Cont;
                for( Cont::iterator i = _position_history.begin(); i != _position_history.end(); ++i ) 
                {
                    position_time & elem( *i );
                    elem.position = elem.position - _dist_reset_thresh;
                }
                _position_sum = _position_sum - _dist_reset_thresh;
            }
            int pos_diff = pos - _previous_position;
            _previous_position = pos;
            _position_sum += pos_diff;
            regression_base::update_position(_position_sum);
	}

	tracking_position::tracking_info tracking_position::get_tracking_info()
	{
            regression_result slope_intercept = regression_base::get_regression();
            float stable_position = slope_intercept.slope * (_position_history.back().time) + slope_intercept.intercept;

            tracking_info retval;
            // return the stable position
            retval.position = stable_position;
            //return regressed velocity of the box too
            retval.velocity = slope_intercept.slope;

            return retval;
	}
        
        float tracking_position::get_velocity()
	{
            return regression_base::get_regression().slope;
	}

	void tracking_position::reset_anchor()
	{
            _previous_position = _anchor_pos;
	}
}