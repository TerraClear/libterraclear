#include "velocity_tracker.hpp"
#include "math.h"

namespace terraclear
{
	velocity_tracker::velocity_tracker(int id, int starting_ypos)
	{
                _frame_count = 0;
                _fps_sum = 0;
		_tracker_id = id;
                _frame_locations[0] = starting_ypos;
		_current_frame_number = 1;
                _sw.start();
                _sw.reset();
	}

	int velocity_tracker::get_id()
	{
		return _tracker_id;
	}

	void velocity_tracker::update_position(int ypos)
	{
                // Adjust calc for how quickly we see new frames
                _frame_count++;
                _fps_sum = _fps_sum + (1.0 / (_sw.get_elapsed_ms() / 1000.0));
//                std::cout << "avg fps: " << _fps_sum / _frame_count << std::endl;
                
                // Update pixel y position for nth frame seen since last reset;
                //then increment n for next frame
		_frame_locations[_current_frame_number] = ypos;
		_current_frame_number ++;
                _sw.reset();
	}

	float velocity_tracker::get_velocity()
	{
		float count = 0.0;  //counter for number of frames
		float x_sum = 0.0;  //sum of x values (frame numbers)
		float y_sum = 0.0;  //sum of y values (y pixel locations)
		for (auto elem: _frame_locations)
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
		for (auto elem : _frame_locations)
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
                //then convert to pixels per second using frames per second
                float pixels_per_frame_slope = r * (s_y / s_x);
                float pixels_per_second = pixels_per_frame_slope * (_fps_sum / _frame_count);
		return pixels_per_second;
	}

	void velocity_tracker::reset_anchor()
	{
		for (auto elem : _frame_locations)
		{
                        //If this round of tracking contained fewer frames
                        //than the previous round, delete the residual frame
                        //entries from the time before
			if (elem.first > _current_frame_number)
			       _frame_locations.erase(elem.first);	
		}
                //Reset to 0 frames seen since last reset
		_current_frame_number = 0;
	}
}
