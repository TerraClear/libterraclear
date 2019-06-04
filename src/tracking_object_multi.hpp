#ifndef TRACKING_POSITION_MULTI_HPP
#define TRACKING_POSITION_MULTI_HPP

#include <cstdlib>
#include <map>
#include <stdint.h>

//opencv
#include "opencv2/opencv.hpp"


#include "tracking_object.hpp"
#include "detector_base.hpp"

namespace terraclear
{    
    class tracking_object_multi 
    {

        public:
            tracking_object_multi();
            tracking_object_multi(int max_sample_queue, int min_track_history, float min_track_velocity, int max_missing_predictions);
            virtual ~tracking_object_multi();
            
            std::vector<bounding_box> track(std::vector<bounding_box> objects, bool remove_missing = true);
            bounding_box predict(int bbox_id);
            
        private:
            struct object_meta
            {
                tracking_object*    obj_ptr = nullptr;
                int                 obj_found_count = 0; 
                int                 obj_lost_count = 0; 
            };
            
            std::map<uint32_t, object_meta> _tracking_list;
            int _max_sample_queue = 10;
            int _min_track_history = 10;
            float _min_track_velocity = 0.0f;
            int _max_missing_predictions = 10;

    };
}
#endif /* TRACKING_POSITION_MULTI_HPP */

