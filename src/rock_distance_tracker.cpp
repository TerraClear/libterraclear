#include "rock_distance_tracker.hpp"

namespace terraclear
{
	rock_distance_tracker::rock_distance_tracker(int id, int queue_size, int starting_pos, int dest_pos, float time_reset_thresh): regression_base(id, queue_size, dest_pos - starting_pos, time_reset_thresh)
	{
            _dest_pos = dest_pos;
	}

	void rock_distance_tracker::update_position(int pos)
	{       
            regression_base::update_position(_dest_pos - pos);
	}

	float rock_distance_tracker::get_distance()
	{
            regression_result slope_intercept = regression_base::get_regression();
            float stable_position = slope_intercept.slope * (_position_history.back().time) + slope_intercept.intercept;
            return stable_position;
	}
}
