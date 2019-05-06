#include "velocity_calculator.hpp"

namespace terraclear
{
    velocity_calculator::velocity_calculator(int start_fps)
    {
        _starting_fps = start_fps;
    }
    
    void velocity_calculator::add_tracker(int id, int starting_ypos)
    {
        velocity_tracker* new_tracker = new velocity_tracker(id, starting_ypos);
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
        float num_trackers = _trackers.size();
        float velocity_sum = 0;
        
        // Add velocity calculations for each individual object in _trackers
        for (auto elem: _trackers)
        {
            velocity_sum = velocity_sum + elem.second->get_velocity();
//            std::cout << "Vel: " << elem.second->get_velocity() << std::endl;
        }
        
        // Compute and return average velocity across all _trackers in pixels
        //per second
        return velocity_sum / num_trackers;
    }
}
