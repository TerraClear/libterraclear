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

#include "thread_base.hpp"

namespace  terraclear
{  
    thread_base::thread_base() 
    {

    }

    thread_base::~thread_base() 
    {   
        pthread_mutex_destroy(&_mutex);
    }
    
    
    void thread_base::thread_start(std::string threadName)
    {
        _threadName = threadName;
        _threadRunning = true;

        pthread_create(&_thread_main, NULL, thread_run, this);
        std::cout << ">>> STARTED THREAD: " << _threadName << std::endl;
    }

    void* thread_base::thread_run(void* thread_obj)
    {
        thread_base* ctxt = (thread_base*) thread_obj;        
        
        while (ctxt->_threadRunning)
        {
            ctxt->thread_runloop();
        }
        
        return nullptr;     
    }

    void thread_base::thread_stop() 
    {
        std::cout << "Thread Stopping: " << _threadName << std::endl;        
        _threadRunning = false;
    }

    void thread_base::thread_stopwait()
    {
        thread_stop(); 
        pthread_join(_thread_main, NULL);
    }
    
    void thread_base::mutex_lock()
    {
        pthread_mutex_lock(&_mutex);
        
    }
    
    void thread_base::mutex_unlock()
    {
       pthread_mutex_unlock(&_mutex);       
    }

}

