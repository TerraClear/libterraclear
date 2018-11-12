/*
 * Base C++ cross platform threading class 
 * Copyright (C) 2017 TerraClear, Inc.
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
*/

#ifndef THREAD_BASE_HPP
#define THREAD_BASE_HPP

#include <stdio.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <unistd.h>

namespace  terraclear
{  
    class thread_base 
    {
        public:
            thread_base();
            virtual ~thread_base();

            void thread_start(std::string threadName);
            void thread_stopwait();
            bool isrunning();
            
            void thread_pause();
            void thread_resume();
            bool ispaused();
            
            std::string get_name();
            
            //lock/unlock resources
            std::mutex* get_mutex_ptr();
            void mutex_lock();
            void mutex_unlock();

        protected:
            //pure virtual function for thread loop..
            virtual void thread_runloop() = 0;
            void thread_stop();
            
        private:
            bool _threadRunning = false;
            bool _threadPaused = false;
            std::string _threadName = "thread_base";
            std::mutex _mutex;
            std::thread* _pthread_main = nullptr;
            
            std::condition_variable _internal_wait_lock;
            std::mutex _internal_mutex;
            
            static void thread_run(thread_base* ctxt);
    };
   
}

#endif /* THREAD_BASE_HPP */

