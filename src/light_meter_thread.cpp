/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   light_meter_thread.cpp
 * Author: izzy
 * 
 * Created on November 4, 2019, 8:50 AM
 */

#ifdef TC_USE_SERIAL

#include "light_meter_thread.hpp"

namespace terraclear
{
    light_meter_thread::light_meter_thread()
    {
        _light_meter.open("/dev/ttyACM0", Baud::BAUD_9600);
    }

    std::string light_meter_thread::get_light_info()
    {
        std::string updated_info;

        mutex_lock();
        updated_info = _back_buffer;
        mutex_unlock();

        return updated_info;
    }

    void light_meter_thread::thread_runloop()
    {
        usleep(100000);
        std::string light_info = _light_meter.readstring(128, 500);
        
        if (light_info.length() == LIGHT_READING_LENGTH)
        {
            mutex_lock();
            _back_buffer = light_info;
            mutex_unlock();
        }
    }
}

#endif