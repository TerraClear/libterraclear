#include "tracking_object_multi.hpp"

namespace terraclear
{   
    tracking_object_multi::tracking_object_multi() 
    {
        
    }

    tracking_object_multi::tracking_object_multi(int max_sample_queue, int min_track_history, float min_track_velocity, int max_prediction_distance)
    {
        _max_sample_queue = max_sample_queue;
        _min_track_history = min_track_history;
        _min_track_velocity = min_track_velocity;
        _max_prediction_distance = max_prediction_distance;
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

    std::vector<bounding_box> tracking_object_multi::track(std::vector<bounding_box> objects, bool remove_missing, uint32_t min_abs_x_v, uint32_t min_abs_y_v)
    {
        std::vector<bounding_box> tracked_list;
        std::vector<uint32_t> tracked_keys;
        
        //add all new objects 
        for (auto bbox : objects)
        {
            //only track/update non-predictions..
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

                    //update object position & get regressed linear velocity
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
                    obj.obj_ptr = new tracking_object(bbox, _max_sample_queue);
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
                else if(keypair.second.obj_predict_distance >= _max_prediction_distance)
                {
                    _tracking_list.erase(keypair.first);
                }
                //remove if we dont have enough history to predict.
                else if (_tracking_list[keypair.first].obj_found_count < _min_track_history)
                {
                    _tracking_list.erase(keypair.first);
                }
                //remove if we dont have enough velocity required to predict.
                else if ((abs(keypair.second.obj_ptr->get_velocity_x()) < min_abs_x_v) ||
                        (abs(keypair.second.obj_ptr->get_velocity_y()) < min_abs_y_v) ) 
                {
                    _tracking_list.erase(keypair.first);
                }
                //predict!
                else
                {
                    //get current box
                    bounding_box current_box = _tracking_list[keypair.first].obj_ptr->get_object();
                    
                    //predict next position
                     _tracking_list[keypair.first].obj_ptr->predict();
                    
                     //get prediction box
                     bounding_box predicted_box = _tracking_list[keypair.first].obj_ptr->get_object();
                     
                     //calculate and increment linear traveled distance
                     int linear_distance = sqrt(abs(pow(current_box.x - predicted_box.x, 2)) + abs(pow(current_box.y - predicted_box.y, 2)));
                     
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

