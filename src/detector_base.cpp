/*
 * base class for detectors
 * Copyright (C) 2019 TerraClear, Inc.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 * CREATED BY: Koos du Preez - koos@terraclear.com
 * 
*/

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

    //save all bounding boxes in standard format for training and validation..
    void  detector_base::saveBoxesJSON(std::string image_file_name, uint32_t image_width, uint32_t image_height, std::list<bounding_box> bboxes)
    {      
        std::string json_file_name = filetools::replace_extension(image_file_name, "json");

        //Generate JSON
        Json::Value root_json;

        root_json["image_name"] = image_file_name;
        root_json["image-width"] = image_width;
        root_json["image-height"] = image_height;

        //array node for boxes                       
        Json::Value bboxes_json(Json::arrayValue);

        //iterate though all  boxes and add to array
        for (auto bbox_tmp : bboxes)
        {        
                Json::Value bbox_json;
                bbox_json["left"] = bbox_tmp.x;
                bbox_json["top"] = bbox_tmp.y;
                bbox_json["width"] = bbox_tmp.width;
                bbox_json["height"] = bbox_tmp.height;

                char tmpstr[100];
                sprintf(tmpstr,"%.0f", bbox_tmp.confidence * 100 );
                bbox_json["confidence"] = std::string(tmpstr);

                bboxes_json.append(bbox_json);
        }

        //add array object into root.
        if (bboxes.size() > 0)
            root_json["bboxes"] = bboxes_json;

        std::string json_string = root_json.toStyledString();

        std::ofstream json_file(json_file_name);
        json_file << json_string;    
        json_file.close();

    }


}