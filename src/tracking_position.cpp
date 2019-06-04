#include "tracking_position.hpp"
#include "math.h"

namespace terraclear
{
	tracking_position::tracking_position(int queue_size)
	{
                _queue_size = queue_size;
                _sw.start();
                _sw.reset();
	}

	void tracking_position::update(int new_position)
	{       
            //reset timer if this is the first update
            //also seed 1st position for a start point to regress
            if (_positions.size() == 0)
            {
                _positions.push_back(new_position);
                _intervals_ms.push_back(0.0f);
                _sw.reset();
            }
            
            //store time since previous update
            uint64_t interval_ms = _sw.get_elapsed_ms();
            _sw.reset();
            
            // Update pixel positions & intervals
            _positions.push_back(new_position);
            _intervals_ms.push_back(interval_ms);
            if (_positions.size() > _queue_size)
            {
                _positions.pop_front();
                _intervals_ms.pop_front();
            }
	}

	tracking_position::tracking_info tracking_position::get_tracking_info()
	{
                float stable_position = 0.0f;
                float pixels_per_second = 0.0f;
                
		float count = _positions.size();  //counter for number of frames
		float x_sum = 0.0;  //sum of x values (frame numbers)
		float y_sum = 0.0;  //sum of y values (y pixel locations)
		for (int i = 0; i < _positions.size(); i++)
		{
			x_sum += i;
			y_sum += _positions[i];
		}

		float m_x = x_sum / count;  //mean x value
	        float m_y = stable_position = y_sum / count;  //mean y value

		float s_x_sum = 0;  //sum of (x-M_x)**2 values for S_x calc
		float s_y_sum = 0;  //sum of (y-M_y)**2 values for S_y calc
		float xy_sum = 0;   //sum of x*y values for r calc

		float x_minus_m = 0;    //for intermediate calc step in loop
		float y_minus_m = 0;    //for intermediate calc step in loop
		for (int i = 0; i < _positions.size(); i++)
		{
			x_minus_m = i - m_x;
			y_minus_m = _positions[i] - m_y;
			s_x_sum += std::pow(x_minus_m, 2);
			s_y_sum += std::pow(y_minus_m, 2);
			xy_sum += (x_minus_m * y_minus_m);	
		}

                //calculate slope of regression only if sums are non zero..
                if ((s_x_sum != 0) && (s_y_sum != 0) && (xy_sum != 0))
                {
                    //Use sums to calculate S_x, S_y, and r values
                    float s_x = sqrt(s_x_sum / (count - 1));
                    float s_y = sqrt(s_y_sum / (count - 1));
                    float r = xy_sum / sqrt(s_x_sum * s_y_sum);

                    //Calc slope of regression (velocity) in pixels per frame
                    //then convert to pixels per second using frames per second
                    float position_slope = r * (s_y / s_x);

                    //sum of all intervals.
                    uint64_t _interval_sum  = 0;
                    for (auto interval_ms : _intervals_ms)
                        _interval_sum += interval_ms;

                    //calculate velocity in pixels per second.if at least 1ms has elapsed.
                    float updates_per_second = 1000.0f / ((float)_interval_sum / (float)_intervals_ms.size());
                    pixels_per_second = (_interval_sum < 1) ? 0 : position_slope * updates_per_second;

                    float distances_intercept = (y_sum - position_slope * x_sum) / _positions.size();
                    stable_position = position_slope * (_positions.size() - 1) + distances_intercept;
                }

                tracking_info retval;
                retval.position = stable_position;
                retval.velocity = pixels_per_second;
                
		return retval;
	}
}
