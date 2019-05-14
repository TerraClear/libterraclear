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

#include <iostream>
#include <list>

//libterraclear
#include "thread_base.hpp"
#include "filetools.hpp"

#include <opencv2/opencv.hpp>

#ifndef CAMERA_RECORDER_HPP
#define CAMERA_RECORDER_HPP

#define CAM_WIDTH 1280
#define CAM_HEIGHT 720

namespace  terraclear
{  

    class camera_recorder : public thread_base
    {
        public:
            camera_recorder();
            virtual ~camera_recorder();
            
            void start_recorder(std::string video_filename, int video_fps, cv::Size video_size);
            void add_frame(cv::Mat video_frame);
            void stop_recorder();
            void pause_recorder();
            void resume_recorder();
            
            std::string get_filename();
            
        protected:
            //pure virtual function implementation..
            virtual void thread_runloop();
            
        private:            
            //default is MP4 video at 99% quality
            std::string _video_filename = "outfile.mp4";
            int _video_format =  cv::VideoWriter::fourcc('M', 'P', '4', 'V');
            float _video_quality = 99.0f;
            int _video_fps = 30;
            cv::Size _video_size;
            
            //frame queue for videos
            std::list<cv::Mat> _video_frames;
            std::condition_variable _frame_notifier;


            //opencv recorder
            bool _is_recording = false;
            cv::VideoWriter _video_recorder;
            
            //initialize video recorder
            void init_recorder();

    };
}
#endif /* CAMERA_RECORDER_HPP */

