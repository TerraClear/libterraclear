/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   light-meter-camera-calibrator.cpp
 * Author: izzy
 * 
 * Created on October 10, 2019, 11:29 AM
 */

#ifdef TC_USE_SERIAL

#include "light_meter_camera_calibrator.hpp"

namespace terraclear
{
    light_meter_camera_calibrator::light_meter_camera_calibrator(int fps)
    {
        _light_meter.open("/dev/ttyACM0", Baud::BAUD_9600);
        _max_exposure_time = (1.0 / fps) * 1000000;
    }

    camera_settings light_meter_camera_calibrator::calibrate_camera_settings()
    {
        camera_settings settings_results;
        
        std::string light_info = _light_meter.readstring(128, 500);
        
        if (light_info.length() == LIGHT_READING_LENGTH)
        {
            light_readings parsed_info = parse_light_readings(light_info);

            settings_results.calibrated = true;
            settings_results.red_multiplier = 1 / (RED_CONST * ((float)parsed_info.red_channel / (float)parsed_info.green_channel));
            settings_results.blue_multiplier = 1 / (BLUE_CONST * ((float)parsed_info.blue_channel / (float)parsed_info.green_channel));
            settings_results.exposure_time = std::exp(MODEL_INTERCEPT) * (std::pow(parsed_info.lux, MODEL_SLOPE));
            
            if (settings_results.exposure_time > _max_exposure_time)
            {
                settings_results.exposure_time = _max_exposure_time;
                settings_results.max_exposure = true;
            }
            
            if (settings_results.red_multiplier > 4)
            {
                settings_results.red_multiplier = 4;
            }
            if (settings_results.blue_multiplier > 4)
            {
                settings_results.blue_multiplier = 4;
            }
        }
        
        return settings_results;
    }
    
    light_readings light_meter_camera_calibrator::parse_light_readings(std::string light_info)
    {
        light_readings results;
        results.cct = std::stoi(light_info.substr(0,5));
        results.lux = std::stoi(light_info.substr(6,5));
        results.red_channel = std::stoi(light_info.substr(12,5));
        results.green_channel = std::stoi(light_info.substr(18,5));
        results.blue_channel = std::stoi(light_info.substr(24,5));
        results.w = std::stoi(light_info.substr(30,5));

        return results;
    }
}

#endif