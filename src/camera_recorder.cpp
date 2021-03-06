/*
 * Base C++ cross platform video recorder class
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

#include "camera_recorder.hpp"
#include "camera_file.hpp"

namespace  terraclear
{  
    camera_recorder::camera_recorder() 
    {
        _video_size.height = CAM_HEIGHT;
        _video_size.width = CAM_WIDTH;
    }

    camera_recorder::~camera_recorder() 
    {
        if (_is_recording)
            stop_recorder();
    }

    std::string camera_recorder::get_filename()
    {
        return _video_filename;
    }


    void camera_recorder::start_recorder(std::string video_filename, int video_fps, cv::Size video_size)
    {
        //generate filename with numeric sequence..
        _video_filename = filetools::generate_filename_seq(video_filename);
        _video_fps = video_fps;
        _video_size = video_size;

        //init recorder
        init_recorder();
        
        //start recorder thread..
        thread_start(_video_filename);

    }
    
    void camera_recorder::init_recorder()
    {
        _is_recording = true;
        _video_recorder.open(_video_filename, _video_format, _video_fps, _video_size);
    }
    
    void camera_recorder::add_frame(cv::Mat video_frame)
    {
        //dont accept any frames for buffer if paused..
        if (!ispaused())
        {
            //if not running, auto start with default values..
            if (!_is_recording)
            {
                init_recorder();
                thread_start(_video_filename);
            }

            //lock buffer list - AUTO unlock when out of scope..
            std::lock_guard<std::mutex> guard(_internal_mutex);

            cv::Mat tmp_frame(video_frame.rows, video_frame.cols, CV_8UC3);
            video_frame.copyTo(tmp_frame);
            _video_frames.push_front(tmp_frame);
            _frame_notifier.notify_one();            
        }


    }
    
    void camera_recorder::stop_recorder()
    {
        //stop writing to file, then release..
        thread_stopwait();
        
        //thread stopped, end file..
        _is_recording = false;
        _video_recorder.release();
    }

    void camera_recorder::pause_recorder()
    {
        thread_pause();
    }
    
    void camera_recorder::resume_recorder()
    {
        thread_resume();        
    }

    void camera_recorder::thread_runloop()
    {

        //creates unique lock obj that can be used for thread signalling..
        std::unique_lock<std::mutex> lk(_internal_mutex);

        // acquire lock, check condition
        // if condition NOT met, release lock and wait for notify or timeoout
        // if condition was not net and now waiting.. re-check condition on notify or timeout, re-lock and continue..
        // if condition IS met, immediately return value and re-lock and continue
        // NOTE - unique lock will auto unlock if locked when out of scope.. 
        bool has_frames = _frame_notifier.wait_for(lk, std::chrono::milliseconds(250), [this]
        {
            return !this->_video_frames.empty();    
        });
        
        //write frames if queue is not empty and wait-lock was notified or condition was "not empty"
        if(has_frames)
        {
            //loop through all items in queue..
            while (_video_frames.size() > 0)
            {
                //get frame, append to file and pop off queue..
                _video_recorder.write(_video_frames.back());
                _video_frames.pop_back();
            }
        }
    }
}