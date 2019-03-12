/*
 * Specific implementation for LUCID Triton camera class 
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

#include "camera_lucid_triton.hpp"
#ifdef TC_USE_TRITON

namespace terraclear
{  

    camera_lucid_triton::camera_lucid_triton()
    {
        //defaults
        _lucid_pixel_format = LUCID_PixelFormat::BayerRG8;
        
        //init lucid system
        init_lucid_system();
        
        //init and start default camera 0
        init_camera(0); 
        
    }

    camera_lucid_triton::~camera_lucid_triton() 
    {
        if (!_disposing)
        {
            _disposing = true;
            
            //stop streaming & cleanup
            _lucid_cam->StopStream();
            _lucid_system->DestroyDevice(_lucid_cam);
            Arena::CloseSystem(_lucid_system);        
        }
    }
    
    error_base camera_lucid_triton::get_generic_error(std::string err_string)
    {
        std::stringstream strstrm;
        strstrm << _base_errstr << err_string;
        return error_base(strstrm.str());
    }
    
    void camera_lucid_triton::init_lucid_system()
    {
        try
        {
            
            //smartptr to lucid subsystem..
            _lucid_system =  Arena::OpenSystem();

            //scan all interfaces
            _lucid_system->UpdateDevices(_timeout_ms);            

            //get lucid devices attached to system
            _lucid_camera_list = _lucid_system->GetDevices();    
            
        }
        catch (GenICam::GenericException& ge)
        {
            throw get_generic_error(ge.what());
        }        
    }

    uint32_t camera_lucid_triton::get_camera_count()
    {
        return _lucid_camera_list.size();
    }
    
    void camera_lucid_triton::init_camera(uint32_t camera_index)
    {
        uint32_t cam_count = get_camera_count();
        if (cam_count <= camera_index)
        {
            throw get_generic_error("Camera Index Out Of Range");
        }
        else
        {
            try
            {
                //get camera at index 0
                _lucid_cam = _lucid_system->CreateDevice(_lucid_camera_list[camera_index]);

                // Set width and height to default
                Arena::SetNodeValue<int64_t>(_lucid_cam->GetNodeMap(), "Width", (int64_t) LUCID_WIDTH); 
                Arena::SetNodeValue<int64_t>(_lucid_cam->GetNodeMap(), "Height", (int64_t) LUCID_HEIGHT); 

                //set acquisition mode.
                Arena::SetNodeValue<GenICam::gcstring>(_lucid_cam->GetNodeMap(), "AcquisitionMode", "Continuous");

                //get required pixel format.
                std::string pixel_format = GetPixelFormatName(_lucid_pixel_format); 

                //set format
                Arena::SetNodeValue<GenICam::gcstring>(_lucid_cam->GetNodeMap(), "PixelFormat", pixel_format.c_str()); 

                //set buffer mode newest only.
                Arena::SetNodeValue<GenICam::gcstring>(_lucid_cam->GetTLStreamNodeMap(), "StreamBufferHandlingMode", "NewestOnly");

                // Set minimum packet delay StreamChannelPacketDelay
                GenApi::CIntegerPtr pStreamChannelPacketDelay = _lucid_cam->GetNodeMap()->GetNode("GevSCPD");
                pStreamChannelPacketDelay->SetValue(pStreamChannelPacketDelay->GetMin());
                
                // Set throughput reserve at 5%.
                GenApi::CIntegerPtr pDeviceLinkThroughputReserve = _lucid_cam->GetNodeMap()->GetNode("DeviceLinkThroughputReserve");
                uint64_t reserve_max = pDeviceLinkThroughputReserve->GetMax();
                pDeviceLinkThroughputReserve->SetValue(reserve_max * 0.05);

               // Set width and height to half
                Arena::SetNodeValue<int64_t>(_lucid_cam->GetNodeMap(), "Width", (int64_t) LUCID_WIDTH/2); 
                Arena::SetNodeValue<int64_t>(_lucid_cam->GetNodeMap(), "Height", (int64_t) LUCID_HEIGHT/2); 

                // fps
                double fps = 125;
                bool set_fps = true;
                float exposure_time = 987625/fps;
                std::cout << exposure_time << std::endl;

                //Force manual framerate - Default is: "Off"
                Arena::SetNodeValue<bool> (_lucid_cam->GetNodeMap(), "AcquisitionFrameRateEnable", set_fps);
                
                if (set_fps)
                {
                    GenApi::CFloatPtr pAcquisitionFrameRate = _lucid_cam->GetNodeMap()->GetNode("AcquisitionFrameRate");
                    pAcquisitionFrameRate->SetValue(fps);
                }                
                
                //Force AE Continuous - Default is: "Continuous"
                Arena::SetNodeValue<GenICam::gcstring>(_lucid_cam->GetNodeMap(), "ExposureAuto", "Off");
        	
                //set exposure manually.
                GenApi::CFloatPtr pExposureTime = _lucid_cam->GetNodeMap()->GetNode("ExposureTime");
                //pExposureTime->SetValue();
                pExposureTime->SetValue(exposure_time);
                
                
                //Start Capture with 10 frame buffer.
                _lucid_cam->StartStream(10);
            
            }
            catch (GenICam::GenericException& ge)
            {
                throw get_generic_error(ge.what());
            }        
        }        
    }
    
    void camera_lucid_triton::update_frames()
    {        
        //acquire image
        Arena::IImage* image_ptr = _lucid_cam->GetImage(_timeout_ms);

        if (image_ptr->IsIncomplete())
        {
            std::cout << "GetNextImage Error - LUCID Image Incomplete." << std::endl;

        }
        else
        {
            //convert to BGR8 for opencv
            Arena::IImage* img_converted_ptr = Arena::ImageFactory::Convert(image_ptr, BGR8);
            
            uint32_t xpad = img_converted_ptr->GetPaddingX();
            uint32_t ypad = img_converted_ptr->GetPaddingX();
            uint32_t width = img_converted_ptr->GetWidth();
            uint32_t height = img_converted_ptr->GetHeight();
            
            //image data contains padding. When allocating Mat container size, you need to account for the X,Y image data padding.
            cv::Mat tmp_mat = cv::Mat(height + ypad, width + xpad, CV_8UC3, (void*)img_converted_ptr->GetData());
            tmp_mat.copyTo(_frame_color);
            tmp_mat.release();
            
        }
        //flush & queue same buffer
        _lucid_cam->RequeueBuffer(image_ptr);

    }

}

#endif //TC_USE_TRITON
