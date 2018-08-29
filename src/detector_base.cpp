#include "detector_base.hpp"
#include <algorithm> 

namespace terraclear
{
    detector_base::detector_base(cv::Mat imgsrc)
    {
        _imgsrc = imgsrc;
    }

    detector_base::~detector_base() 
    {
    }
    

    void detector_base::mergeBoundingBoxes(std::vector<bounding_box> &object_boxes, uint32_t expand_pixels)
    {
        //fill grow/expand all boxes if needed
        if (expand_pixels > 0)
        {            
            int half_expand = expand_pixels / 2;
            for (auto &bbox : object_boxes)
            {

                    bbox.x = bbox.x - std::min(half_expand, bbox.x);
                    bbox.y = bbox.y - std::min(half_expand, bbox.y);
                    bbox.width += expand_pixels;
                    bbox.height += expand_pixels;
            }
        }
        
        //list of clusters
        std::vector<bounding_box_cluster> cluster_list;
        
        //cluster all overlapping boxes.
        int cluster_global_index = 1;
        for (auto it = object_boxes.cbegin(); it != object_boxes.cend(); ) // not hoisted & no increment
        {
            //de-ref it.
            auto bbox_orig =  *it;

            //erase it
            object_boxes.erase(it);
            
            //list of keeper candidates
            std::vector<bounding_box> candidate_list;
            candidate_list.push_back(bbox_orig);
            
            //find all overlapping boxes from remainder boxes
            for (auto &bbox_overlap : object_boxes)
            {
                //check overlap area, if overlapped, add to list for cluster
                if (getIntersectRect(bbox_orig, bbox_overlap).area() > 0)
                {
                    candidate_list.push_back(bbox_overlap);
                }
            }   
            
            //check if any of candidates belong to any existing clusters
            int cluster_local_index = 0;
            for (auto candidate : candidate_list)
            {
                for (auto cluster : cluster_list)
                {
                    for (auto clustered_box : cluster.clustered_boxes)
                    {
                        //found candidate in existing cluster.. assign all..
                        if (candidate.track_id == clustered_box.track_id)
                        {
                            cluster_local_index = cluster.cluster_id;                            
                            cluster.clustered_boxes.insert(cluster.clustered_boxes.end(), candidate_list.begin(), candidate_list.end());
                            break;
                        }
                    }
                }
            }
            
            //create new cluster if no existing..
            if (cluster_local_index == 0)
            {
                bounding_box_cluster bbox_cluster;
                bbox_cluster.cluster_id = cluster_global_index;
                bbox_cluster.clustered_boxes.insert(bbox_cluster.clustered_boxes.end(), candidate_list.begin(), candidate_list.end());
                cluster_global_index ++;
                
                cluster_list.push_back(bbox_cluster);
            }
        }       
        
        //create bounding boxes from clusters..
        for (auto cluster : cluster_list)
        {
            int tlx = 999999;
            int tly = 999999;
            int brx = 0;
            int bry = 0;

            for (auto clustered_box : cluster.clustered_boxes)
            {
                tlx = std::min(tlx, clustered_box.x);
                tly = std::min(tly, clustered_box.y);
                brx = std::max(brx, clustered_box.x + clustered_box.width);
                bry = std::max(bry, clustered_box.y + clustered_box.height);
            }
            
            bounding_box supercluster;
            supercluster.x = tlx;
            supercluster.y = tly;
            supercluster.width = brx - tlx;
            supercluster.height = bry - tly;
            supercluster.track_id = cluster.cluster_id;
            
            //Shrink if boxes were grown...
            if (expand_pixels > 0)
            {
                int half_expand = expand_pixels / 2;

                supercluster.x = supercluster.x + half_expand;
                supercluster.y = supercluster.y + half_expand;
                supercluster.width -= expand_pixels;
                supercluster.height -= expand_pixels;
            }

            object_boxes.push_back(supercluster);
            
        }
        
     }    
    
    
    cv::Rect detector_base::getIntersectRect(bounding_box b1, bounding_box b2)
    {
        return b1 & b2; // & returns rect intersect 
    }
}