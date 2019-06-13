#include "velocity_tracker.hpp"

namespace terraclear
{
	velocity_tracker::velocity_tracker(int id, int queue_size, int starting_pos, float time_reset_thresh, int dist_reset_thresh): regression_base(id, queue_size, 0, time_reset_thresh)
	{
            _previous_position = starting_pos;
            _position_sum = 0;
            _anchor_pos = starting_pos;
            _dist_reset_thresh = dist_reset_thresh;
	}

	void velocity_tracker::update_position(int pos)
	{
            //If distance traveled getting too large, set values back
            if (_position_history.front().position > _dist_reset_thresh)
            {   
                typedef std::list< position_time > Cont;
                for( Cont::iterator i = _position_history.begin(); i != _position_history.end(); ++i ) {
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

	float velocity_tracker::get_velocity()
	{
            return regression_base::get_regression().slope;
	}

	void velocity_tracker::reset_anchor()
	{
            _previous_position = _anchor_pos;
	}
}
