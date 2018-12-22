#ifndef DETECTOR_BASE_HPP
#define DETECTOR_BASE_HPP

#include <vector>
#include <map>

//jsoncpp https://linux.tips/programming/how-to-install-and-use-json-cpp-library-on-ubuntu-linux-os
#ifdef __linux__
    #include <jsoncpp/json/json.h>
#else
    #include <json/json.h>
#endif

//OPENCV
#include <opencv2/opencv.hpp>

//internal includes
#include "filetools.hpp"


namespace terraclear
{
    struct bounding_box : cv::Rect
    {
        float confidence;                 
        unsigned int class_id; 
        std::string class_string;
        unsigned int track_id;        
        unsigned int frame_count;    
        float distance_cm;
        
        // < operator overload for sorting in Y (top to bottom)
        bool operator < (const bounding_box &compare_box) const 
        {
            return (y + height) < (compare_box.y + compare_box.height);
        }

        // > operator overload for sorting.
        bool operator > (const bounding_box &compare_box) const 
        {
            return (y + height) > (compare_box.y + compare_box.height);
        }

    };

    struct bounding_box_cluster
    {
        uint32_t cluster_id;
        std::vector<bounding_box> clustered_boxes;
    };

    class detector_base 
    {
        public:

            detector_base(cv::Mat imgsrc);
            virtual ~detector_base();

            //pure virtual 
            virtual std::vector<bounding_box> detect_objects() = 0;

            static void saveBoxesJSON(std::string image_file_name, uint32_t image_width, uint32_t image_height, std::list<bounding_box> bboxes);
       
        protected:
            cv::Mat _imgsrc;
            void mergeBoundingBoxes(std::vector<bounding_box> &object_boxes, uint32_t expand_pixels = 0);
            cv::Rect getIntersectRect(bounding_box b1, bounding_box b2);   
           
    };

}
#endif /* DETECTOR_BASE_HPP */

