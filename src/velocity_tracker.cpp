#include "velocity_tracker.hpp"
#include "math.h"

namespace terraclear
{
	velocity_tracker::velocity_tracker(uint32_t fps, uint32_t id)
	{
		_fps = fps;
		_tracker_id = id;
		_current_frame_number = 0;
	}

	uint32_t velocity_tracker::get_id()
	{
		return _tracker_id;
	}

	void velocity_tracker::update_position(uint32_t ypos)
	{
		_frame_locations[_current_frame_number] = ypos;
		_current_frame_number ++;
	}

	uint32_t velocity_tracker::find_velocity()
	{
		float count = 0.0;
		float x_sum = 0.0;
		float y_sum = 0.0;
		for (auto elem: _frame_locations)
		{
			count++;
			x_sum += elem.first;
			y_sum += elem.second;
		}

		float m_x = x_sum / count;
	        float m_y = y_sum / count;	

		float s_x_sum = 0;
		float s_y_sum = 0;
		float xy_sum = 0;

		float x_minus_m = 0;
		float y_minus_m = 0;
		for (auto elem : _frame_locations)
		{
			x_minus_m = elem.first - m_x;
			y_minus_m = elem.second - m_y;
			s_x_sum += x_minus_m**2;
			s_y_sum += y_minus_m**2;
			xy_sum += (x_minus_m * y_minus_m);	
		}

		float s_x = sqrt(s_x_sum / (count - 1));
		float s_y = sqrt(s_y_sum / (count - 1));
		float r = xy_sum / sqrt(s_x_sum * s_y_sum);

		return r * (s_y / s_x);
	}

	void velocity_tracker::reset_anchor()
	{
		for (auto elem : _frame_locations)
		{
			if (elem.first > _current_frame_number)
			       _frame_locations.erase(elem.first);	
		}
		_current_frame_number = 0;
	}
}
