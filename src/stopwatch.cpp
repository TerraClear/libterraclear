/*
 * Basic Stopwatch Timer
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
*/

#include <chrono>

#include "stopwatch.hpp"
namespace  terraclear
{
    stopwatch::stopwatch() 
    {
        _elapsed_us = 0;
        _prev_elapsed_us = 0;
        _isrunning = false;
    }

    stopwatch::~stopwatch() 
    {
    }

    
    void stopwatch::start()
    {
        //reset clock        
        if (!_isrunning)
        {
            _mutex.lock();
                _lastmeasure = std::chrono::steady_clock::now();
                _isrunning = true;
            _mutex.unlock();
        }
        
    }
    
    void stopwatch::stop()
    {
        if (_isrunning)
        {
            _mutex.lock();
                _isrunning = false;
            _mutex.unlock();
            
            update_clock();
        }
    }

    void stopwatch::reset()
    {
        //reset clock 
        _mutex.lock();
            _elapsed_us =_prev_elapsed_us = 0;
            _lap_times.clear();
            _lastmeasure = std::chrono::steady_clock::now();
        _mutex.unlock();
        
        update_clock();
    }
    
    void stopwatch::update_clock()
    {
        //lock and unlock when out of scope..
        std::lock_guard<std::mutex> lk(_mutex); 

        _elapsed_us += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - _lastmeasure).count();        
        _lastmeasure = std::chrono::steady_clock::now();
    }


    uint32_t stopwatch::lap()
    {
        update_clock();

        //lock and unlock when out of scope..
        std::lock_guard<std::mutex> lk(_mutex); 
 
        uint64_t lap_time = _elapsed_us - _prev_elapsed_us;

        laptime lt;
        lt.elapsed_time  = _prev_elapsed_us;
        lt.lap_time  = lap_time;

        //reset for next lap..
        _prev_elapsed_us = _elapsed_us;

        //add lap time to vector.
        _lap_times.push_back(lt);

        return  _lap_times.size() - 1; //return laptime index ID.
    }

    uint64_t stopwatch::get_elapsed_us()
    {
        //total time since last reset..
        update_clock();
        return  _elapsed_us;
    }

    uint64_t stopwatch::get_elapsed_ms()
    {
        return get_elapsed_us() / 1000;
    }

    uint64_t stopwatch::get_elapsed_s()
    {
        return get_elapsed_us() / 1000000;
    }


    stopwatch::laptime stopwatch::get_lap_us(uint32_t lap_id)
    {
        //lock and unlock when out of scope..
        std::lock_guard<std::mutex> lk(_mutex); 

        laptime lt = _lap_times.at(lap_id);
        return lt;
    }

    stopwatch::laptime stopwatch::get_lap_ms(uint32_t lap_id)
    {
        laptime lt = get_lap_us(lap_id);

        //lock and unlock when out of scope..
        std::lock_guard<std::mutex> lk(_mutex); 

        lt.elapsed_time /= 1000;
        lt.lap_time /= 1000;

        return lt;
    }

    stopwatch::laptime stopwatch::get_lap_s(uint32_t lap_id)
    {
        laptime lt = get_lap_us(lap_id);

        //lock and unlock when out of scope..
        std::lock_guard<std::mutex> lk(_mutex); 

        lt.elapsed_time /= 1000000;
        lt.lap_time /= 1000000;

        return lt;
    }


}
