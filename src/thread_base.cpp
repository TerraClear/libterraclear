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

#include <unistd.h>

#include "thread_base.hpp"

namespace  terraclear
{  
    thread_base::thread_base() 
    {

    }

    thread_base::~thread_base() 
    {   
        //kill thread if still running..
        if (_threadRunning)
        {
            _threadRunning = false;
            pthread_join(_thread_main, NULL);
        }

        pthread_mutex_destroy(&_mutex);
    }
    
    
    void thread_base::thread_start(std::string threadName)
    {
        _threadName = threadName;

        pthread_create(&_thread_main, NULL, thread_run, this);

        _threadRunning = true;
    }

    void* thread_base::thread_run(void* thread_obj)
    {
        thread_base* ctxt = static_cast<thread_base*>(thread_obj);        
        
        while (ctxt->_threadRunning)
        {
            if (!ctxt->_threadPaused)
            {
                try
                {
                    ctxt->thread_runloop();
                }
                catch (std::exception& e)
                {
                    std::cout << std::endl << " ****** " << ctxt->get_name() << " Exception caught : " << e.what() << std::endl;
                    ctxt->thread_stop();
                }

            }
            else
            {
                usleep(1000);// thread paused...
            }
        }
        
        return nullptr;     
    }

    void thread_base::thread_stop() 
    {
        _threadRunning = false;
    }

    void thread_base::thread_stopwait()
    {
        thread_stop(); 
        pthread_join(_thread_main, NULL);
    }
    
    bool thread_base::isrunning()
    {
        return _threadRunning;
    }

    void thread_base::thread_pause()
    {
        _threadPaused =_threadPaused = true;
    }
    
    void thread_base::thread_resume()
    {
        _threadPaused =_threadPaused = false;        
    }
    
    bool thread_base::ispaused()
    {
        return _threadPaused;
    }
            
    std::string thread_base::get_name()
    {
        return _threadName;
    }
     
    void thread_base::mutex_lock(pthread_mutex_t* lockable_mutex)
    {
        pthread_mutex_lock(lockable_mutex);
        
    }

    void thread_base::mutex_lock()
    {
        mutex_lock(&_mutex);
    }
    
    void thread_base::mutex_unlock(pthread_mutex_t* lockable_mutex)
    {
       pthread_mutex_unlock(lockable_mutex);       
    }

    void thread_base::mutex_unlock()
    {
        mutex_unlock(&_mutex);
        
    }

    pthread_mutex_t* thread_base::get_mutex_ptr()
    {
        return &_mutex;
                
    }
}

