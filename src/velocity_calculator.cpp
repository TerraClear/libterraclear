/*
 * helper class for tracking velocity of pixels in pixels per second 
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

#include "velocity_calculator.hpp"

namespace terraclear
{
    velocity_calculator::velocity_calculator(int start_fps)
    {
        _starting_fps = start_fps;
    }
    
    void velocity_calculator::add_tracker(int id, int starting_pos)
    {
        velocity_tracker* new_tracker = new velocity_tracker(id, starting_pos);
        _trackers[id] = new_tracker;
    }
    
    void velocity_calculator::remove_tracker(int id) 
    {
        if (_trackers.count(id) > 0)
            _trackers.erase (id); 
    }

    void velocity_calculator::update_tracker_position(int id, int current_pos)
    {
        if (_trackers.count(id) > 0)
            _trackers[id]->update_position(current_pos);
    }
    
    void velocity_calculator::reset_tracker_anchor(int id)
    {
        if (_trackers.count(id) > 0)
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
