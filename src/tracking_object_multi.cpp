#include "tracking_object_multi.hpp"

namespace terraclear
{   
    tracking_object_multi::tracking_object_multi() 
    {
        
    }

    tracking_object_multi::tracking_object_multi(int max_sample_queue, int min_track_history, float min_track_velocity, int max_missing_predictions)
    {
        _max_sample_queue = max_sample_queue;
        _min_track_history = min_track_history;
        _min_track_velocity = min_track_velocity;
        _max_missing_predictions = max_missing_predictions;
    }
    
    tracking_object_multi::~tracking_object_multi() 
    {
//        //delete all trackers
//        for (auto const& keypair : _tracking_list)
//            delete keypair.second;
    }

    std::vector<bounding_box> tracking_object_multi::track(std::vector<bounding_box> objects, bool remove_missing)
    {
        std::vector<bounding_box> tracked_list;
        std::vector<uint32_t> tracked_keys;
        
        //add all new objects and remove or predict missing.
        for (auto bbox : objects)
        {
            // if it exists, just update it else create new tracker and add to tracker list,.        
            if (_tracking_list.count(bbox.track_id) > 0)
            {
                //increment times obj has been seen by tracker
                _tracking_list[bbox.track_id].obj_found_count++;
                
                //update seen count..
                bbox.frame_count = _tracking_list[bbox.track_id].obj_found_count;

                //update object position & get regressed linear velocity
                _tracking_list[bbox.track_id].obj_ptr->update(bbox);
                
                //start using regressed positions when at least max tracked positions updated
                if (bbox.frame_count > _max_sample_queue)
                {
                    //bbox = _tracking_list[bbox.track_id].obj_ptr->get_current();
                    bbox.velocity_x =  _tracking_list[bbox.track_id].obj_ptr->get_velocity_x();
                    bbox.velocity_y =  _tracking_list[bbox.track_id].obj_ptr->get_velocity_y();
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
        
        //isolate missing objects and if required reomve or predict....        
        for (auto keypair : _tracking_list)
        {
            //check if item present in current list
            if (std::find(tracked_keys.begin(), tracked_keys.end(), keypair.first) == tracked_keys.end())
            {
                //remove if force remove enabled or max predictions reached or not enough history present..
                if (    (remove_missing) || 
                        (keypair.second.obj_lost_count >= _max_missing_predictions) || 
                        (_tracking_list[keypair.first].obj_found_count < _min_track_history))
                {
                    _tracking_list.erase(keypair.first);
                }
                else
                {
                    //get prediction
                    bounding_box predicted_box =  _tracking_list[keypair.first].obj_ptr->get_predicted();

                    //increment counts & update
                     _tracking_list[keypair.first].obj_lost_count ++;
                     _tracking_list[keypair.first].obj_found_count++;
                     
                     //update position using predicted location..
                     _tracking_list[keypair.first].obj_ptr->update(predicted_box);

                    //add prediction to tracked list..
                    tracked_list.push_back(predicted_box);  
                    
//                    std::cout << "[" << keypair.first << "-" << _tracking_list[keypair.first].obj_found_count;
//                    std::cout << ",x_v=" << _tracking_list[keypair.first].obj_ptr->get_velocity_x();
//                    std::cout << ",y_v=" << _tracking_list[keypair.first].obj_ptr->get_velocity_y();
//                    std::cout <<  "]" << std::endl <<  std::flush;
                }               
            } 
            
        }
        
        
        
        
        return tracked_list;
    }

    bounding_box tracking_object_multi::predict(int bbox_id)
    {        
        bounding_box bbox;
        bbox.x = bbox.y = bbox.width = bbox.height = 0;
        
        //ensure its tracked
        if (_tracking_list.count(bbox_id) > 0)
        {
            bbox = _tracking_list.at(bbox_id).obj_ptr->get_predicted();
        } 
        
        return bbox;
    }
    
}

