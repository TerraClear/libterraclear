#include "rock_distance_tracker.hpp"
#include "math.h"

namespace terraclear
{
	rock_distance_tracker::rock_distance_tracker(int starting_ypos, int dest_ypos, int queue_size)
	{
                _fps_sum = 0;
                _dest_ypos = dest_ypos;
                _queue_size = queue_size;
                _distances.push_back(dest_ypos - starting_ypos);
	}

	void rock_distance_tracker::update_position(int ypos)
	{       
                // Update pixel y position for nth frame seen since last reset;
                //then increment n for next frame
                _distances.push_back(_dest_ypos - ypos);
                if (_distances.size() > _queue_size)
                {
                    _distances.pop_front();
                }
	}

	float rock_distance_tracker::get_distance()
	{
		float count = _distances.size();  //counter for number of frames
		float x_sum = 0.0;  //sum of x values (frame numbers)
		float y_sum = 0.0;  //sum of y values (y pixel locations)
		for (int i = 0; i < _distances.size(); i++)
		{
			x_sum += i;
			y_sum += _distances[i];
		}

		float m_x = x_sum / count;  //mean x value
	        float m_y = y_sum / count;  //mean y value

		float s_x_sum = 0;  //sum of (x-M_x)**2 values for S_x calc
		float s_y_sum = 0;  //sum of (y-M_y)**2 values for S_y calc
		float xy_sum = 0;   //sum of x*y values for r calc

		float x_minus_m = 0;    //for intermediate calc step in loop
		float y_minus_m = 0;    //for intermediate calc step in loop
		for (int i = 0; i < _distances.size(); i++)
		{
			x_minus_m = i - m_x;
			y_minus_m = _distances[i] - m_y;
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
                float distances_slope = r * (s_y / s_x);
                float distances_intercept = (y_sum - distances_slope * x_sum) / _distances.size();
                float stable_position = distances_slope * (_distances.size() - 1) + distances_intercept;
                //TEST
//                std::cout << "slope: " << distances_slope << ", int: " << distances_intercept << std::endl;
		return stable_position;
	}
}
