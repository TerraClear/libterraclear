#include "velocity_calculator.hpp"

namespace terraclear
{
    velocity_calculator::velocity_calculator(int start_fps, float time_reset_thresh, int dist_reset_thresh)
    {
        _starting_fps = start_fps;
        _time_reset_thresh = time_reset_thresh;
        _dist_reset_thresh = dist_reset_thresh;
    }
    
    void velocity_calculator::add_tracker(int id, int queue_size, int starting_ypos)
    {
        terraclear::regression_obj_meta info;
        info.bbox_id = id;
        info.queue_size = queue_size;
        info.starting_pos = starting_ypos;
        info.dist_reset_thresh = _dist_reset_thresh;
        
        velocity_tracker* new_tracker = new velocity_tracker(info);
        _trackers[id] = new_tracker;
    }
    
    void velocity_calculator::update_tracker_position(int id, int ypos)
    {
        _trackers[id]->update_position(ypos);
    }
    
    void velocity_calculator::reset_tracker_anchor(int id)
    {
        _trackers[id]->reset_anchor();
    }
    
    float velocity_calculator::get_average_velocity()
    {
        float velocity_sum = 0.0f;
        
        // Add velocity calculations for each individual object in _trackers
        for (auto elem: _trackers)
        {
            velocity_sum = velocity_sum + elem.second->get_velocity();
        }
        
        // Compute and return average velocity across all _trackers in pixels
        //per second
        return velocity_sum / _trackers.size();
    }
}
