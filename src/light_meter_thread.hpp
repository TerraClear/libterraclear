/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   light_meter_thread.hpp
 * Author: izzy
 *
 * Created on November 4, 2019, 8:50 AM
 */

#ifdef TC_USE_SERIAL

//libterraclear
#include "thread_base.hpp"
#include "basicserial.hpp"

#include <unistd.h>

#ifndef LIGHT_METER_THREAD_HPP
#define LIGHT_METER_THREAD_HPP

#define LIGHT_READING_LENGTH 35

namespace  terraclear
{  
    class light_meter_thread : public thread_base
    {
        public:
            light_meter_thread();
            std::string get_light_info();
            
        protected:
            virtual void thread_runloop();
            
        private:
            basicserial _light_meter;
            std::string _back_buffer;
    };
}

#endif /* LIGHT_METER_THREAD_HPP */

#endif /* TC_USE_SERIAL */