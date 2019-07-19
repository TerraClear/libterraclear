/*
 * helper class for calculating velocity of tracked areas in pixels per second 
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
 * CREATED BY: Isabelle Butterfield - isabelle.butterfield@terraclear.com
 * 
*/

#include "camera_velocity_calculator.hpp"

namespace terraclear
{
    bool camera_velocity_calculator::get_tracked_anchor(std::vector<bbox_t> &bbox_list, bbox_t &anchor)
    {
        bool retval = false;

        for (auto bbox : bbox_list)
        {
            //found it.. update anchor..
            if (bbox.track_id == anchor.track_id)
            {
                //update anchor..
                anchor.x = bbox.x;
                anchor.y = bbox.y;
                anchor.w = bbox.w;
                anchor.h = bbox.h;
                anchor.frames_counter = bbox.frames_counter;

                retval = true;
                break;                 
            }
        }

        return retval;
    }

    camera_velocity_calculator::camera_velocity_calculator(cv::Size dst_size, int track_start_y, int paddle_offset, int track_max_travel, int track_xy_size, float time_reset_thresh, int dist_reset_thresh, int anchor_queue_size)
    {
        _track_xy_size = track_xy_size;
        _track_max_travel = track_max_travel;
        _tracker_engine = new Tracker_optflow(0, 21, 6, 8000, -1);
                
        uint32_t    track_count = 6;
        uint32_t    paddle_lane = dst_size.height - paddle_offset;
        uint32_t    track_offset_x = dst_size.width / track_count;
        uint32_t    track_offset_y_calc = ((paddle_lane - track_start_y)/2) / (track_count + 4);
        uint32_t    box_x = track_xy_size/2;
        uint32_t    box_y = track_start_y;
        uint32_t    box_y_2 = track_start_y + ((paddle_lane - track_start_y)/2);

        
        _calculator_x_v = new velocity_calculator(0, time_reset_thresh, dist_reset_thresh);
        _calculator_y_v = new velocity_calculator(0, time_reset_thresh, dist_reset_thresh);
        
        for (uint32_t t = 0; t < track_count; t++ )
        {
            bbox_t tmp_box;
            tmp_box.x = box_x;
            tmp_box.y = box_y;
            tmp_box.w = tmp_box.h = track_xy_size;
            tmp_box.track_id = t;

            //keep track of anchors by ID
            _track_anchors.push_back(tmp_box);

            box_y += track_offset_y_calc;

            //Add tracker with bbox ID to collection for averaging velocity
            _calculator_x_v->add_tracker(tmp_box.track_id, anchor_queue_size, tmp_box.x);
            _calculator_y_v->add_tracker(tmp_box.track_id, anchor_queue_size, tmp_box.y);
            
            bbox_t tmp_box2;
            tmp_box2.x = box_x;
            tmp_box2.y = box_y_2;
            tmp_box2.w = tmp_box2.h = track_xy_size;
            tmp_box2.track_id = t + track_count;

            //keep track of anchors by ID
            _track_anchors.push_back(tmp_box2);

            box_x += track_offset_x;
            box_y_2 += track_offset_y_calc;

            //Add tracker with bbox ID to collection for averaging velocity
            _calculator_x_v->add_tracker(tmp_box2.track_id, anchor_queue_size, tmp_box2.x);
            _calculator_y_v->add_tracker(tmp_box2.track_id, anchor_queue_size, tmp_box2.y);

            //start with anchor boxes
            _tracker_engine->update_cur_bbox_vec(_track_anchors);
        }
        return;
    }
    
    std::vector<bbox_t> camera_velocity_calculator::update_tracking(cv::Mat new_img)
    {
        std::vector<bbox_t> track_boxes_new;
        // Make sure the image isnt blank. If is, dont do a tracking flow update
        
        cv::Scalar sum = cv::sum(
                         cv::sum(new_img.row((int)(new_img.rows-1))) + 
                         cv::sum(new_img.row((int)(new_img.rows-2))) + 
                         cv::sum(new_img.row((int)(new_img.rows-3)))
                        );
        if (sum != cv::Scalar(0))
        {
            // No blank frame
             _track_boxes = _tracker_engine->tracking_flow(new_img);
            //ensure anchors were tracked and not lost or past limits..
            for (auto anchor : _track_anchors)
            {
                //check that original anchor has been tracked
                bbox_t tmp_bbox = anchor;
                if (get_tracked_anchor(_track_boxes, tmp_bbox))
                {
                    //if anchor tracked and not past travel limits, keep tracking
                    //else reset back to anchor.. ..
                    if ((tmp_bbox.y + _track_xy_size / 2) < (anchor.y + _track_max_travel))
                    {
                        //keep tracked box
                        track_boxes_new.push_back(tmp_bbox);
                        _calculator_x_v->update_tracker_position(tmp_bbox.track_id, tmp_bbox.x);
                        _calculator_y_v->update_tracker_position(tmp_bbox.track_id, tmp_bbox.y);

                    }
                    else
                    {
                        //reset to anchor..
                        track_boxes_new.push_back(anchor);
                        _calculator_x_v->reset_tracker_anchor(tmp_bbox.track_id);
                        _calculator_y_v->reset_tracker_anchor(tmp_bbox.track_id);
                    }
                }
                else
                {
                    //add un-tracked or lost anchor..
                    track_boxes_new.push_back(anchor);
                }

                //update tracker engine with corrected box positions..
                _tracker_engine->update_cur_bbox_vec(track_boxes_new);
            } 
        }
        else
        {
            // Blank frame, probably
        }
       
        return track_boxes_new;
    }
    
    std::vector<bbox_t> camera_velocity_calculator::get_anchor_tracks()
    {
        return _track_anchors;
    }
    
    float camera_velocity_calculator::get_frame_x_v()
    {
        return _calculator_x_v->get_average_velocity();
    }
    
    float camera_velocity_calculator::get_frame_y_v()
    {
        return _calculator_y_v->get_average_velocity();
    }
    
    float camera_velocity_calculator::get_frame_x_a()
    {
        return _calculator_x_a->get_average_velocity();
    }
        
    float camera_velocity_calculator::get_frame_y_a()
    {
        return _calculator_y_a->get_average_velocity();
    }
   
}
