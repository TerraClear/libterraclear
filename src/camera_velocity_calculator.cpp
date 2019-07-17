#include "camera_velocity_calculator.hpp"

namespace terraclear
{
    camera_velocity_calculator::camera_velocity_calculator(cv::Size dst_size, int track_start_y, int track_end_y, int track_max_travel, int track_offset_y, int track_xy_size, float time_reset_thresh, int dist_reset_thresh)
    {
        _track_xy_size = track_xy_size;
        _track_max_travel = track_max_travel;
        _tracker_engine = new Tracker_optflow(0, 21, 6, 8000, -1);
        
        uint32_t    track_count = 1.5 * (track_end_y - track_start_y) / (track_max_travel - track_offset_y);
        uint32_t    track_offset_x = dst_size.width / (track_count + 1);
        uint32_t    box_x = track_offset_x;
        uint32_t    box_y = track_start_y;
        
        _calculator = new velocity_calculator(0, time_reset_thresh, dist_reset_thresh);
        
        for (uint32_t t = 0; t < track_count; t++ )
        {
            bbox_t tmp_box;
            tmp_box.x = box_x - (track_xy_size / 2);
            tmp_box.y = box_y - (track_xy_size / 2);
            tmp_box.w = tmp_box.h = track_xy_size;
            tmp_box.track_id = t;

            //keep track of anchors by ID
            _track_anchors.push_back(tmp_box);

            box_x += track_offset_x;
            box_y += track_offset_y;

            //Add tracker with bbox ID to collection for averaging velocity
            _calculator->add_tracker(tmp_box.track_id, 30, tmp_box.y);

            //start with anchor boxes
            _tracker_engine->update_cur_bbox_vec(_track_anchors);
        }
        
        
    }
    
    void camera_velocity_calculator::update_tracking(cv::Mat new_img, bool draw_tracking_info)
    {
        //do tracking of boxes
        _track_boxes = _tracker_engine->tracking_flow(new_img);
        
        //ensure anchors were tracked and not lost or past limits..
        std::vector<bbox_t> track_boxes_new;
        for (auto anchor : _track_anchors)
        {
            //get anchor, check if tracked..
            bbox_t tmp_bbox = anchor;
            if (get_tracked_anchor(_track_boxes, tmp_bbox))
            {
                //if anchor tracked and not past travel limits, keep tracking
                //else reset back to anchor.. ..
                if ((tmp_bbox.y + _track_xy_size / 2) < (anchor.y + _track_max_travel))
                {
                    //keep tracked box
                    track_boxes_new.push_back(tmp_bbox);

                    _calculator->update_tracker_position(tmp_bbox.track_id, tmp_bbox.y);
                }
                else
                {
                    //reset to anchor..
                    track_boxes_new.push_back(anchor);
                    _calculator->reset_tracker_anchor(tmp_bbox.track_id);
                }
            }
            else
            {
                //add un-tracked or lost anchor..
                track_boxes_new.push_back(anchor);
            }

            //update tracker engine with corrected box positions..
            _tracker_engine->update_cur_bbox_vec(track_boxes_new);
            // Draw tracking if configured
            if (draw_tracking_info)
            {
                //draw the anchor start and end lines
                cv::Point bbox_start(anchor.x + anchor.h / 2, anchor.y + anchor.w / 2);
                cv::Point bbox_end(bbox_start.x, bbox_start.y + _track_max_travel);
                line(new_img, bbox_start, bbox_end, cv::Scalar(0, 0xff, 0x00), 2);

                //draw tracked areas..
                cv::Rect bbox_rect;
                bbox_rect.x = tmp_bbox.x;
                bbox_rect.y = tmp_bbox.y;
                bbox_rect.width = tmp_bbox.w;
                bbox_rect.height = tmp_bbox.h;
                cv::rectangle(new_img, bbox_rect, cv::Scalar(255,255,255), 2);
            }
        }    
        return;
    }
    
    float camera_velocity_calculator::get_frame_velocity()
    {
        return _calculator->get_average_velocity();
    }
    
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
}

