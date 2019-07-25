/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   paddles.h
 * Author: anishau
 *
 * Created on July 24, 2019, 1:41 PM
 */

#ifndef PADDLES_H
#define PADDLES_H

struct paddles{
    
    std::vector<cv::Vec3d> rvec;
    std::vector<cv::Vec3d> tvec;
    std::vector<cv::Point3d> real_coords;
    int num_paddles;
    
};

#endif /* PADDLES_H */

