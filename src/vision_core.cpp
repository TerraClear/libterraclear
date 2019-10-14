/*
 * core tools & utils
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

#include <string>

#include "vision_core.hpp"



namespace terraclear
{

    vision_core::vision_core() 
    {
    }


    vision_core::~vision_core() 
    {
    }
    
    vision_class_type vision_core::classStringToType(std::string in_class_string)
    {   
        //default is background..
        vision_class_type retval = vision_class_type::background;
        
        //class types are always lower case.
        std::string cls_str = in_class_string;
        std::transform(cls_str.begin(), cls_str.end(), cls_str.begin(), ::tolower); 
        
        //find match 
        if (cls_str.compare(VISION_CLASS_ROCK) == 0)
            retval = vision_class_type::rock;
        else if (cls_str.compare(VISION_CLASS_NOT_ROCK) == 0) 
            retval = vision_class_type::not_rock;
        
        return retval;        
    }
    
    std::string vision_core::classTypeToString(vision_class_type in_class_type)
    {
        //default is background..
        std::string retval = VISION_CLASS_BACKGROUND;
        
        switch (in_class_type)
        {
            case vision_class_type::rock:
                retval = VISION_CLASS_ROCK;
                break;
            case vision_class_type::not_rock:
                retval = VISION_CLASS_NOT_ROCK;
                break;
            default:
                break;
        }
        
        return retval;
    }
    
    void vision_core::mergeBoxes(std::vector<bounding_box> &object_boxes, uint32_t expand_pixels)
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
            
        } //end for clusters        
     }    
        
    
    cv::Rect vision_core::getIntersectRect(bounding_box b1, bounding_box b2)
    {
        return b1 & b2; // & returns rect intersect 
    }
    
    //save all bounding boxes in standard format for training and validation..
    void  vision_core::saveBoxesJSON(std::string image_file_name, uint32_t image_width, uint32_t image_height, std::list<bounding_box> bboxes)
    {      
        std::string json_file_name = filetools::replace_extension(image_file_name, "json");

        //Generate JSON
        Json::Value root_json;

        root_json["ImageName"] = image_file_name;
        root_json["imageWidth"] = image_width;
        root_json["imageHeight"] = image_height;

        //array node for boxes                       
        Json::Value bboxes_json(Json::arrayValue);

        //iterate though all  boxes and add to array
        for (bounding_box bbox_tmp : bboxes)
        {        
                Json::Value bbox_json;
                bbox_json["left"] = bbox_tmp.x;
                bbox_json["top"] = bbox_tmp.y;
                bbox_json["width"] = bbox_tmp.width;
                bbox_json["height"] = bbox_tmp.height;

                char tmpstr[100];
                sprintf(tmpstr,"%.0f", bbox_tmp.confidence * 100 );
                bbox_json["confidence"] = std::string(tmpstr);
              
                bbox_json["label"] = vision_core::classTypeToString(bbox_tmp.class_id);
                
                if (bbox_tmp.detected)
                    bbox_json["detected"] = true;
                if (bbox_tmp.annotated)
                    bbox_json["annotated"] = true;

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
    

    std::list<bounding_box> vision_core::readBoxesJSON(std::string json_file_name, uint32_t min_area)
    {
        std::list<bounding_box> bboxes;

        //If bboxes json file exist, load them into vector..
        if (filetools::file_exists(json_file_name.c_str()))
        {
            //read json file..
            Json::Value json_obj;
            std::ifstream json_fstream(json_file_name, std::ifstream::binary);
            json_fstream >> json_obj;        

            //get Bounding Boxes for img
            Json::Value bboxes_json = json_obj["bboxes"];

            //iterate and create box vector
            for (Json::Value::ArrayIndex i = 0; i != bboxes_json.size(); i++)
            {
                bounding_box bbox;

                bbox.width =  bboxes_json[i].get("width", 0).asInt();
                bbox.height = bboxes_json[i].get("height", 0).asInt();           
                bbox.x = bboxes_json[i].get("x", 0).asInt();
                bbox.y = bboxes_json[i].get("y", 0).asInt();

                //support for json format where left  = x and y top = y
                bbox.x = (bbox.x > 0) ? bbox.x :  bboxes_json[i].get("left", 0).asInt();
                bbox.y = (bbox.y > 0) ? bbox.y :  bboxes_json[i].get("top", 0).asInt();

                bbox.confidence = (float)std::stoi(bboxes_json[i].get("confidence", "1000").asCString()) / 100.0f;
                bbox.class_id = classStringToType(bboxes_json[i].get("label", VISION_CLASS_ROCK).asCString());

                //Add bbox to labeled boxes if min area size
                if ((bbox.width * bbox.height) >= min_area)
                    bboxes.push_back(bbox);
            }

            json_fstream.close();
        }

        return bboxes;
    }

std::list<bounding_box> vision_core::readBoxesTXT(std::string txtfilepath, int image_width, int image_height, uint32_t min_area)
{
    std::list<bounding_box> bboxes;
    
    //If bboxes TXT file exist, load them into vector..
    if (filetools::file_exists(txtfilepath.c_str()))
    {
        // bounding boxes
        std::ifstream infile(txtfilepath.c_str());
        int classtype;
        float fcenterx, fcentery, fboxw, fboxh;

        //read all lines from file and create bounding boxes. 
        while (infile >> classtype >> fcenterx >> fcentery >> fboxw >> fboxh)
        {
            bounding_box bbox;

            bbox.width =  abs(image_width * fboxw);
            bbox.height =  abs(image_height * fboxh);
            unsigned int icenterx =  abs(image_width * fcenterx);
            unsigned int icentery =  abs(image_height * fcentery); 
            bbox.x = icenterx - bbox.width / 2;
            bbox.y = icentery - bbox.height / 2;
            bbox.confidence = 1.00f;
            bbox.class_id = (vision_class_type) classtype;

            //Add bbox to labeled boxes if min area size
            if ((bbox.width * bbox.height) >= min_area)
                bboxes.push_back(bbox);

        }
    }//endif TXT file..

    return bboxes;
}


}
