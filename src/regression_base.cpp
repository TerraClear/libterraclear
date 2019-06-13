#include "regression_base.hpp"
#include "math.h"

namespace terraclear
{
	regression_base::regression_base(int id, int queue_size, int starting_pos, float time_reset_thresh)
	{
            _id = id;
            _queue_size = queue_size;
            _time_sum = 0;
            _time_reset_thresh = time_reset_thresh;
            
            position_time init_pos;
            init_pos.time = _time_sum;
            init_pos.position = starting_pos;
            _position_history.push_back(init_pos);
            
            _sw.start();
            _sw.reset();
	}

	void regression_base::update_position(int pos)
	{
            
            position_time current_pos;
            _time_sum = _time_sum + _sw.get_elapsed_ms() / 1000.0;
            _sw.reset();
            current_pos.time = _time_sum;
            current_pos.position = pos;

            //Add pixel position for current frame;
            //then remove element at front of list
            _position_history.push_back(current_pos);
            if (_position_history.size() > _queue_size)
            {
                _position_history.pop_front();
            }

            //If accumulating time getting too large, set values back
            if (_position_history.front().time > _time_reset_thresh)
            {   
                typedef std::list< position_time > Cont;
                for( Cont::iterator i = _position_history.begin(); i != _position_history.end(); ++i ) {
                    // dereference the iterator to get a reference to the element
                    position_time & s( *i );
                    s.time = s.time - _time_reset_thresh;
                }
                _time_sum = _time_sum - _time_reset_thresh;
            }
	}

	regression_result regression_base::get_regression()
	{
		float count = 0.0;  //counter for number of frames
		float x_sum = 0.0;  //sum of x values (frame numbers)
		float y_sum = 0.0;  //sum of y values (y pixel locations)
                
		for (auto elem : _position_history)
		{
			count++;
			x_sum += elem.time;
			y_sum += elem.position;
		}

		float m_x = x_sum / count;  //mean x value
	        float m_y = y_sum / count;  //mean y value

		float s_x_sum = 0;  //sum of (x-M_x)**2 values for S_x calc
		float xy_sum = 0;   //sum of x*y values for r calc

		float x_minus_m = 0;    //for intermediate calc step in loop
		float y_minus_m = 0;    //for intermediate calc step in loop
		for (auto elem : _position_history)
		{
			x_minus_m = elem.time - m_x;
			y_minus_m = elem.position - m_y;
			s_x_sum += std::pow(x_minus_m, 2);
			xy_sum += (x_minus_m * y_minus_m);	
		}
                
                //Calculate slope of regression (velocity) in pixels per second
                float slope = xy_sum / s_x_sum;
                float intercept = m_y - slope * m_x;
                
                regression_result slope_intercept;
                slope_intercept.slope = slope;
                slope_intercept.intercept = intercept;
		return slope_intercept;
	}
}
