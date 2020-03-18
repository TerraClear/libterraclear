#include "tracking_object_multi.hpp"
namespace tc = terraclear;
namespace terraclear
{   
    tracking_object_multi::tracking_object_multi() 
    {
        
    }

    tracking_object_multi::tracking_object_multi(int max_sample_queue, int min_track_history, float min_track_velocity, int max_prediction_distance, int max_zero_velocity_count, int paddle_line)
    {
        _max_sample_queue = max_sample_queue;
        _min_track_history = min_track_history;
        _min_track_velocity = min_track_velocity;
        _max_prediction_distance = max_prediction_distance;
        _max_zero_vel_count = max_zero_velocity_count;
        _paddle_line = paddle_line;
    }
    
    tracking_object_multi::~tracking_object_multi() 
    {
//        //delete all trackers
//        for (auto const& keypair : _tracking_list)
//            delete keypair.second;
    }
    
    bool tracking_object_multi::boxes_contain_point(cv::Point source_point, std::vector<bounding_box> target_boxes)
    {
        bool found_box = false;

        //check if point within a bounding pox collection
        for (auto tmp_box : target_boxes)
        {
            if (tmp_box.contains(source_point))
            {
                found_box = true;
                break;
            }
        }
        
        return found_box;
    }    

    std::vector<bounding_box> tracking_object_multi::track(std::vector<bounding_box> objects, uint32_t min_abs_x_v, uint32_t min_abs_y_v, float frame_v_x, float frame_v_y, bool use_frame_v, bool remove_missing)
    {
        std::vector<bounding_box> tracked_list;
        std::vector<uint32_t> tracked_keys;
        
        //add all new objects 
        for (auto bbox : objects)
        {
            //only predict/update non-predictions..
            if (!bbox.predicted)
            {
                // if it exists, just update it else create new tracker and add to tracker list,.        
                if (_tracking_list.count(bbox.track_id) > 0)
                {
                    //increment times obj has been seen by tracker
                    _tracking_list[bbox.track_id].obj_found_count++;

                    //reset predicted distance
                    _tracking_list[bbox.track_id].obj_predict_distance = 0;

                    //update seen count..
                    bbox.frame_count = _tracking_list[bbox.track_id].obj_found_count;
                    
                    //update object position to get the stabilized position and velocity
                    _tracking_list[bbox.track_id].obj_ptr->update(bbox);
                    
                    //start using regressed positions when at least max tracked positions updated
                    if (bbox.frame_count > _min_track_history)
                    {
                        //bbox = _tracking_list[bbox.track_id].obj_ptr->get_current();
                        bbox.velocity_x =  _tracking_list[bbox.track_id].obj_ptr->get_velocity_x();
                        bbox.velocity_y =  _tracking_list[bbox.track_id].obj_ptr->get_velocity_y();
                    }
                    else
                    {
                        bbox.velocity_x = 0;
                        bbox.velocity_y = 0;
                    }  
                }            
                else
                {
                    //create new object to be tracked..
                    object_meta obj;
                    
                    regression_obj_meta regression_obj_y;
                    regression_obj_y.bbox = bbox;
                    regression_obj_y.bbox_id = bbox.track_id;
                    regression_obj_y.dest_pos = 1200;
                    regression_obj_y.queue_size = 30;
                    regression_obj_y.starting_pos = bbox.y;
                    regression_obj_y.time_reset_thresh = 4.0f;

                    regression_obj_meta regression_obj_x = regression_obj_y;
                    regression_obj_x.starting_pos = bbox.x;

                    
                    // Creates ptr to object tracking class
                    obj.obj_ptr = new tracking_object(regression_obj_x, regression_obj_y);
                    obj.obj_found_count = bbox.frame_count = 1; //tracker has seen it for the first time..
                    _tracking_list[bbox.track_id] = obj;
                }
                //add to tracked list..
                tracked_list.push_back(bbox);
                tracked_keys.push_back(bbox.track_id);
            }
        }
        
        //isolate missing objects and if required remove or predict....        
        for (auto keypair : _tracking_list)
        {
            //check if item NOT present in current list
            if (std::find(tracked_keys.begin(), tracked_keys.end(), keypair.first) == tracked_keys.end())
            {
                //remove if we are not supposed to be predicting.any missing objects.
                if (remove_missing) 
                {
                    _tracking_list.erase(keypair.first);
                }
                //remove if enough predictions
                else if (keypair.second.obj_predict_distance >= _max_prediction_distance)
                {
                    _tracking_list.erase(keypair.first);
                }
                // remove if we dont have enough history.
                else if (_tracking_list[keypair.first].obj_found_count < _min_track_history)
                { 
                    _tracking_list.erase(keypair.first);
                }
                //Remove if min velocity not reached. 
                else if ((abs(keypair.second.obj_ptr->get_velocity_x()) < min_abs_x_v) ||
                        (abs(keypair.second.obj_ptr->get_velocity_y()) < min_abs_y_v) ) 
                {
                    _tracking_list.erase(keypair.first);
                }
                else if (_tracking_list[keypair.first].obj_ptr->_zero_vel_count > _max_zero_vel_count)
                {
                    _tracking_list.erase(keypair.first);
                }
                //predict!
                else
                {
                    //get current box
                    bounding_box current_box = _tracking_list[keypair.first].obj_ptr->get_object();
                    
                    _tracking_list[keypair.first].obj_ptr->_frame_x_v = frame_v_x;
                    _tracking_list[keypair.first].obj_ptr->_frame_y_v = frame_v_y;
                    
                    // predict next position using frame velocity
                    _tracking_list[keypair.first].obj_ptr->predict_average();
             
                    //check for zero velocity
                    (keypair.second.obj_ptr->get_velocity_y() == 0) ? _tracking_list[keypair.first].obj_zero_vel_count++ 
                                                                           : _tracking_list[keypair.first].obj_zero_vel_count = 0;
                    
                    //get prediction box
                    tc::bounding_box predicted_box = _tracking_list[keypair.first].obj_ptr->get_object();
                    
                    //calculate and increment linear traveled distance
                    int linear_distance = sqrt(pow(current_box.x - predicted_box.x, 2) + pow(current_box.y - predicted_box.y, 2));

                    //increment counts & update
                    _tracking_list[keypair.first].obj_predict_distance += linear_distance;              
                    _tracking_list[keypair.first].obj_lost_count ++;
                    _tracking_list[keypair.first].obj_found_count++;

                    //add prediction to tracked list..
                    tracked_list.push_back(predicted_box); 
                    
                }               
            } 
        }
        
        return tracked_list;
    }   
}
