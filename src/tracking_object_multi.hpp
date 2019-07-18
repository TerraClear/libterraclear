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
            tracking_object_multi(int max_sample_queue, int min_track_history, float min_track_velocity, int max_prediction_disance);
            virtual ~tracking_object_multi();
            
            std::vector<bounding_box> track(std::vector<bounding_box> objects, bool remove_missing = true, uint32_t min_abs_x_v = 0, uint32_t min_abs_y_v = 0, bool predict_negative_y_v = false, float frame_v_x = 0,float frame_v_y = 0, bool use_frame_v = true);
            
        private:
            struct object_meta
            {
                tracking_object*    obj_ptr = nullptr;
                int                 obj_found_count = 0; 
                int                 obj_lost_count = 0; 
                int                 obj_predict_distance = 0;
            };
            
            std::map<uint32_t, object_meta> _tracking_list;
            int _max_sample_queue = 10;
            int _min_track_history = 0;
            float _min_track_velocity = 0.0f;
            int _max_prediction_distance = 10;
            float _stable_frame_vel = 0;

            bool boxes_contain_point(cv::Point source_point, std::vector<bounding_box> target_boxes);
            
    };
}
#endif /* TRACKING_POSITION_MULTI_HPP */

