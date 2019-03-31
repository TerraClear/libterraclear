#ifndef VISION_WARP_H
#define VISION_WARP_H

#include <vector>
#include <opencv2/imgproc.hpp>
#include "opencv2/calib3d.hpp"

namespace  terraclear
{
    class vision_warp 
    {
        protected:
            struct  roi_transform
            {
                cv::Point2f top_left;
                cv::Point2f top_right;
                cv::Point2f bottom_right;
                cv::Point2f bottom_left;
            };

        public:
            vision_warp();
            virtual ~vision_warp();

            roi_transform   _source_points;
            cv::Size        _target_size;

            void    init_transform();
            cv::Mat transform_image(cv::Mat img_src);

        private:            
            cv::Mat _transform_matrix;

    };
}

#endif /* VISION_WARP_H */
