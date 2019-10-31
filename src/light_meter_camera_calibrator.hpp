/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   light-meter-camera-calibrator.hpp
 * Author: izzy
 *
 * Created on October 10, 2019, 11:29 AM
 */

#ifndef LIGHT_METER_CAMERA_CALIBRATOR_HPP
#define LIGHT_METER_CAMERA_CALIBRATOR_HPP

#ifdef TC_USE_SERIAL

#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include "math.h"

//other libterraclear includes
#ifndef TC_USE_SERIAL
    #define TC_USE_SERIAL
#endif
#include "basicserial.hpp"

#include <json/json.h>

#define OPENCV

#define MODEL_SLOPE -0.92784623
#define MODEL_INTERCEPT 14.14128986
#define RED_CONST 0.7928844
#define BLUE_CONST 0.43118733
#define LIGHT_READING_LENGTH 35

struct camera_settings
{
    
    bool calibrated = false;
    bool max_exposure = false;
    float red_multiplier;
    float blue_multiplier;
    int exposure_time;
    float gain = 0.0;
};

struct light_readings
{
    int cct;
    int lux;
    int red_channel;
    int green_channel;
    int blue_channel;
    int w;
};

namespace terraclear
{
    class light_meter_camera_calibrator
    {
        public:
            light_meter_camera_calibrator(int fps);
            camera_settings calibrate_camera_settings();
            
        private:
            basicserial _light_meter;
            int _max_exposure_time;
            light_readings parse_light_readings(std::string light_info);
    };
    
}

#endif /* TC_USE_SERIAL */

#endif /* LIGHT_METER_CAMERA_CALIBRATOR_HPP */
