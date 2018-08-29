#ifndef DETECTOR_HSV_HPP
#define DETECTOR_HSV_HPP

#include "detector_base.hpp"

namespace terraclear
{

    class detector_hsv : public detector_base
    {
        public:
            //default values for various shades of blue.
            cv::Scalar _lowrange = cv::Scalar(87, 155, 120);
            cv::Scalar _highrange = cv::Scalar(109, 249 , 254);

            detector_hsv(cv::Mat imgsrc);
            virtual ~detector_hsv();

            //pure virtual implementation
            std::vector<bounding_box> detect_objects();

        private:
            

    };
}
#endif /* DETECTOR_HSV_HPP */

