#include "tracking_object.hpp"

namespace terraclear
{    

    tracking_object::tracking_object(bounding_box bbox, int history_depth) 
        : _x_tracker(history_depth), _y_tracker(history_depth)
    {      
        _history_depth = history_depth;
        _bbox = bbox;
        _position_count = 1;
        
        _sw.start();
        _sw.reset();
    }

    tracking_object::~tracking_object() 
    {
    }

    int tracking_object::get_id()
    {
        return _bbox.track_id;
    }

    uint32_t  tracking_object::get_position_count()
    {
        return _position_count;
    }

    void tracking_object::reset()
    {
        //_x_tracker.reset_position();
       // _y_tracker.reset_position();

        _last_heights.clear();
        _last_widths.clear();
        
        _position_count = 0;
        
        _sw.reset();
    }
    
    float tracking_object::get_velocity_x()
    {
       return _x_v;
    }
    
    float tracking_object::get_velocity_y()
    {
        return _y_v;
        
    }
    
    float tracking_object::get_velocity_linear()
    {
        return sqrtf( pow(_x_v, 2) + pow(_y_v, 2) );        
    }
    
    void tracking_object::update(bounding_box bbox)
    {
        _bbox = bbox;

        //update velocity tracker for center points
        _x_tracker.update(_bbox.get_center().x);
        _y_tracker.update(_bbox.get_center().y);
        
        //update with the velocity and stable positions
        tracking_position::tracking_info x_info = _x_tracker.get_tracking_info();
        _x_v = x_info.velocity;
        _bbox.x = x_info.position -_bbox.width / 2;
        
        tracking_position::tracking_info y_info = _y_tracker.get_tracking_info();
        _y_v = y_info.velocity;
        _bbox.y = y_info.position - _bbox.height / 2;
        
        //update object linear velocity & times seen...
        _bbox.velocity_x = _x_v;
        _bbox.velocity_y = _y_v;

        //keep track of width/height history for averaging.
        _last_widths.push_front(bbox.width);
        _last_heights.push_front(bbox.height);

        //only keep last X number of box sizes.
        if (_last_widths.size() > _max_history)
        {
            _last_widths.pop_back();
            _last_heights.pop_back();
        }        
        
        //update object to median size
        _bbox.width = get_list_median(_last_widths);
        _bbox.height = get_list_median(_last_heights);

        //increase positions we've tracked
        _position_count++;

        _sw.reset();
    }

    bounding_box tracking_object::get_current()
    {
        return _bbox;
    }   
    
    bounding_box tracking_object::get_predicted()
    {
        //time passed since previous update.
        float dT =  1000.0f / (float) _sw.get_elapsed_ms();

        //update next prediction..
        _bbox_next = _bbox;

//** TODO fix X tracking.. IGNORE X FOR NOW
        //predict new center position based on velocity
        int new_center_x = _bbox.get_center().x;// + (_x_v / dT);
        int new_center_y = _bbox.get_center().y + (_y_v / dT);
        
        _bbox_next.x = new_center_x - (_bbox_next.width / 2);
        _bbox_next.y = new_center_y - (_bbox_next.height / 2);
        
        return _bbox_next;
    }

    int tracking_object::get_list_median(std::list<int> value_list)
    {
        int retval = 0;

        //need at least 3 for median..
        if (value_list.size() > 2) 
        {
            std::vector<int> value_vector;
            for (auto value_item : value_list)
                value_vector.push_back(value_item);

            //median
             std::nth_element(value_vector.begin(), value_vector.begin() + value_vector.size()/2, value_vector.end());
             retval = value_vector[value_vector.size()/2];
        } 
        else if (value_list.size() > 0)
            //not enough for median, just use last value..
            retval = value_list.back();

        return retval;
    };

}