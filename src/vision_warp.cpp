#include "vision_warp.h"

namespace  terraclear
{
    vision_warp::vision_warp() 
    {
    }

    vision_warp::~vision_warp() 
    {
    }
    
    void vision_warp::init_transform()
    {
        //Source & Dest Points
        std::vector<cv::Point2f> points_src;
        points_src.push_back(_source_points.top_left);        
        points_src.push_back(_source_points.top_right);        
        points_src.push_back(_source_points.bottom_right);        
        points_src.push_back(_source_points.bottom_left); 
        
        //calculate target points based on target size..
        roi_transform target_points;
        target_points.top_left.x = 0;
        target_points.top_left.y = 0;
        target_points.top_right.x = _target_size.width;
        target_points.top_right.y = 0;
        target_points.bottom_right.x = _target_size.width;
        target_points.bottom_right.y = _target_size.height;
        target_points.bottom_left.x = 0;
        target_points.bottom_left.y = _target_size.height;

        
        std::vector<cv::Point2f> points_dst;
        points_dst.push_back(target_points.top_left);        
        points_dst.push_back(target_points.top_right);        
        points_dst.push_back(target_points.bottom_right);        
        points_dst.push_back(target_points.bottom_left); 

        //compute transformation matrix.
        _transform_matrix =  cv::findHomography(points_src, points_dst); 

    }
    
    cv::Mat vision_warp::get_transfor_matrix()
    {
        return _transform_matrix;
    }
            
    cv::Mat vision_warp::transform_image(cv::Mat img_src)
    {
        cv::Mat img_result;
        
        //warp original & resize.
        cv::warpPerspective(img_src, img_result, _transform_matrix, _target_size); // do perspective transformation
      
        return img_result;
    }
    
}
