/*
 * helper class for calculating average velocity using linear regression
 * Copyright (C) 2019 TerraClear, Inc.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 * CREATED BY: Isabelle Butterfield - isabelle.butterdield@terraclear.com
 * 
*/


#include "tracking_velocity.hpp"
#include "math.h"

namespace terraclear
{
	tracking_velocity::tracking_velocity(int id, int starting_pos)
	{
                _position_count = 0;
                _positions_per_sec_sum = 0;
		_tracker_id = id;
                _position_history[0] = starting_pos;
		_last_position_index = 1;
                _sw.start();
                _sw.reset();
	}

	int tracking_velocity::get_id()
	{
		return _tracker_id;
	}

	void tracking_velocity::update_position(int current_pos)
	{
                // Adjust calc for how quickly we see new frames
                _position_count++;
                uint64_t ms = _sw.get_elapsed_ms();
                _positions_per_sec_sum = (ms > 0) ? _positions_per_sec_sum + (1.0 / (ms / 1000.0)) : 0;
                
                // Update position for nth frame seen since last reset;
                //then increment n for next frame
		_position_history[_last_position_index] = current_pos;
		_last_position_index ++;
                _sw.reset();
	}

	float tracking_velocity::get_velocity()
	{
		float count = 0.0;  //counter for number of positional updates
		float x_sum = 0.0;  //sum of x values (positional update numbers)
		float y_sum = 0.0;  //sum of y values (y pixel locations)
		for (auto elem: _position_history)
		{
			count++;
			x_sum += elem.first;
			y_sum += elem.second;
		}

		float m_x = x_sum / count;  //mean x value
	        float m_y = y_sum / count;  //mean y value

		float s_x_sum = 0;  //sum of (x-M_x)**2 values for S_x calc
		float s_y_sum = 0;  //sum of (y-M_y)**2 values for S_y calc
		float xy_sum = 0;   //sum of x*y values for r calc

		float x_minus_m = 0;    //for intermediate calc step in loop
		float y_minus_m = 0;    //for intermediate calc step in loop
		for (auto elem : _position_history)
		{
			x_minus_m = elem.first - m_x;
			y_minus_m = elem.second - m_y;
			s_x_sum += std::pow(x_minus_m, 2);
			s_y_sum += std::pow(y_minus_m, 2);
			xy_sum += (x_minus_m * y_minus_m);	
		}

                //Use sums to calculate S_x, S_y, and r values
		float s_x = sqrt(s_x_sum / (count - 1));
		float s_y = sqrt(s_y_sum / (count - 1));
		float r = xy_sum / sqrt(s_x_sum * s_y_sum);
                
                //Calc slope of regression (velocity) in pixels per frame
                //then convert to pixels per second using updates per second
                float pixels_per_update_slope = r * (s_y / s_x);
                float pixels_per_second = pixels_per_update_slope * (_positions_per_sec_sum / _position_count);
		return pixels_per_second;
	}

	void tracking_velocity::reset_position()
	{
		for (auto elem : _position_history)
		{
                        //If this round of tracking contained fewer frames
                        //than the previous round, delete the residual frame
                        //entries from the time before
			if (elem.first > _last_position_index)
			       _position_history.erase(elem.first);	
		}
                //Reset to 0 frames seen since last reset
		_last_position_index = 0;
                _positions_per_sec_sum = 0;
	}
}
