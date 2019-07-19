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
	}

	void tracking_position::update(int pos)
	{       
            regression_base::update_position(pos);
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
}