/*
 * Specific implementation for FLIR Blackfly S camera class 
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
#ifdef TC_USE_BLACKFLY

#include "camera_flir_blackfly.hpp"

namespace terraclear
{  
    camera_flir_blackfly::camera_flir_blackfly(flir_settings cam_settings)
    {        
        _cam_settings = cam_settings;
        
        //init flir system
        init_flir_system();
        
        //set camera to default..
        _flir_cam = _flir_camera_list.GetByIndex(0);
        init_camera(); 
    }

    camera_flir_blackfly::camera_flir_blackfly(flir_settings cam_settings, uint32_t cam_index)
    {
        _cam_settings = cam_settings;

        //init flir system
        init_flir_system();
        
        //set camera to specific index..
        _flir_cam = _flir_camera_list.GetByIndex(cam_index);
        init_camera(); 
        
    }
    
    camera_flir_blackfly::camera_flir_blackfly(flir_settings cam_settings, std::string cam_serial)
    {
        _cam_settings = cam_settings;

        //init flir system
        init_flir_system();
        
        //set camera to specific serial..
        _flir_cam = _flir_camera_list.GetBySerial(cam_serial);
        init_camera(); 
        
        //validate serial number
        if (_cam_serial.compare(cam_serial) != 0)
            throw get_generic_error("FLIR Camera with Serial " + cam_serial + " not found!");
    }

            
    camera_flir_blackfly::~camera_flir_blackfly() 
    {
        if (!_disposing)
        {
            _disposing = true;
            
        //stop & cleanup camera resources..
        _flir_cam->EndAcquisition();
        _flir_cam->DeInit();

        //clear cameras before releasing system instance..
        _flir_camera_list.Clear();

        //de-ref camera..
        _flir_cam = nullptr;
        
        //release system
        _flir_system->ReleaseInstance();
        
        }
    }
    
    error_base camera_flir_blackfly::get_flir_error(flir::Exception &e)
    {
        std::stringstream strstrm;
        strstrm << FLIR_ERR_STR << e.GetErrorMessage() << ":" << e.GetFunctionName();
        return error_base(strstrm.str(), e.GetError());
    }
    
    error_base camera_flir_blackfly::get_generic_error(std::string err_string)
    {
        std::stringstream strstrm;
        strstrm << FLIR_ERR_STR << err_string;
        return error_base(strstrm.str());
    }
    
    std::vector<std::string> camera_flir_blackfly::get_cameras()
    {
        flir::SystemPtr flir_system = nullptr;
        flir::CameraList flir_camera_list;        

        std::vector<std::string> cam_list;
                
        try
        {
            //smartptr to dlir System..
            flir_system = flir::System::GetInstance();

            //get flir cameras attached to system
            flir_camera_list = flir_system->GetCameras();  
            uint32_t cam_count = flir_camera_list.GetSize();
            for (uint32_t cam_index = 0; cam_index < cam_count; cam_index++)
            {
                std::string cam_serial = flir_camera_list.GetByIndex(cam_index)->GetUniqueID().c_str();
                cam_list.push_back(cam_serial);
            }

            //clear cameras before releasing system instance..
            flir_camera_list.Clear();
            //release system
            flir_system->ReleaseInstance();            

        }
        catch (flir::Exception &e)
        {
            throw get_flir_error(e);
        }      
        
        return cam_list;
        
    }
    
    void camera_flir_blackfly::init_flir_system()
    {
        //defaults
        try
        {
            //smartptr to dlir System..
            _flir_system = flir::System::GetInstance();

            //get flir cameras attached to system
            _flir_camera_list = _flir_system->GetCameras();            
        }
        catch (flir::Exception &e)
        {
            throw get_flir_error(e);
        }        
    }

    uint32_t camera_flir_blackfly::get_camera_count()
    {
        return _flir_camera_list.GetSize();
    }
    
    void camera_flir_blackfly::init_camera()
    {
        
        try
        {
            //if camera not acquired, acquire default 
            if (_flir_cam == nullptr)
                _flir_cam = _flir_camera_list.GetByIndex(0);

            //init camera
            _flir_cam->Init();

            //get nodemaps
            //flir_api::INodeMap& nmTL = pcam->GetTLDeviceNodeMap();
            flir_api::INodeMap& flir_nodemap = _flir_cam->GetNodeMap();

            //Get Serial
            flir_api::CStringPtr p_DeviceID = flir_nodemap.GetNode("DeviceID");
            _cam_serial = p_DeviceID->GetValue();
            
            //get camera in AcquisitionMode mode enumerator value
            flir_api::CEnumerationPtr pAcquisitionMode = flir_nodemap.GetNode("AcquisitionMode");
            int64_t acquisition_continuous = pAcquisitionMode->GetEntryByName("Continuous")->GetValue();

            // Set 'Continuous' value for Acquisition from on acquisition mode
            pAcquisitionMode->SetIntValue(acquisition_continuous);    

            //FlipY
            flir_api::CBooleanPtr pReverseY = flir_nodemap.GetNode("ReverseY");
            pReverseY->SetValue(_cam_settings.flip_y);

            //Set Exposure Mode to Manual exposure (Values are "Off", "Continuous", "Once")
            flir_api::CEnumerationPtr pExposureAuto = flir_nodemap.GetNode("ExposureAuto");
            int exposure_off = pExposureAuto->GetEntryByName("Off")->GetValue();
            pExposureAuto->SetIntValue(exposure_off);

            //Set exposure time..
            flir_api::CFloatPtr pExposureTime = flir_nodemap.GetNode("ExposureTime");
            pExposureTime->SetValue(_cam_settings.exposure_time);

            //Set Binning
            //ensure at least bin 1
            _cam_settings.bin_horizontal = std::max(_cam_settings.bin_horizontal, 1);            
            _cam_settings.bin_vertical = std::max(_cam_settings.bin_vertical, 1);            
            flir_api::CIntegerPtr ptrBinW = flir_nodemap.GetNode("BinningVertical");
            ptrBinW->SetValue(_cam_settings.bin_horizontal);
            flir_api::CIntegerPtr ptrBinH = flir_nodemap.GetNode("BinningHorizontal");
            ptrBinH->SetValue(_cam_settings.bin_vertical);

            //Set Image Width/Height
            flir_api::CIntegerPtr pWidth = flir_nodemap.GetNode("Width");
            pWidth->SetValue(_cam_settings.width);
            flir_api::CIntegerPtr pHeight = flir_nodemap.GetNode("Height");
            pHeight->SetValue(_cam_settings.height);

            //Force Frame Rate
            flir_api::CBooleanPtr ptrFPSEnable = flir_nodemap.GetNode("AcquisitionFrameRateEnable");
            ptrFPSEnable->SetValue(true);
            flir_api::CFloatPtr ptrFPS = flir_nodemap.GetNode("AcquisitionFrameRate");
            ptrFPS->SetValue(_cam_settings.fps);


            //get current pixel format and change if needed..
            flir::PixelFormatEnums flir_format = _flir_cam->PixelFormat.GetValue();
            if (flir_format != _cam_settings.pixel_format)
            {
                //get ptr to pixel format node item
                flir_api::CEnumerationPtr ptrPixelFormat = flir_nodemap.GetNode("PixelFormat");
                
                //format string..
                std::string pixel_format_string = flir_pixel_format_to_string(_cam_settings.pixel_format);

                // Retrieve the desired entry node from the enumeration node
                flir_api::CEnumEntryPtr ptrPixelFormatEntry = ptrPixelFormat->GetEntryByName(pixel_format_string.c_str());
                if (!flir_api::IsAvailable(ptrPixelFormatEntry) || (!flir_api::IsReadable(ptrPixelFormatEntry)))
                {
                    throw get_generic_error("PixelFormat '" + pixel_format_string + "' not available.");
                }
                else
                {
                    // Retrieve the integer value from the entry node
                    int64_t pixelFormatBGR8 = ptrPixelFormatEntry->GetValue();

                    // Set integer as new value for enumeration node
                    ptrPixelFormat->SetIntValue(pixelFormatBGR8);

                }// end pixelformat value readable  

            }// end pixelformat change check

            // begin acquisition.
            _flir_cam->BeginAcquisition();

        }
        catch (flir::Exception &e)
        {
            throw get_flir_error(e);
        }          
    }
    
    void camera_flir_blackfly::update_frames()
    {        
        //acquire image
        flir::ImagePtr image_ptr = _flir_cam->GetNextImage();

        //check if image was complete on grab..
        if (image_ptr->GetImageStatus() !=  flir::ImageStatus::IMAGE_NO_ERROR)
        {
            std::string img_status = std::to_string(image_ptr->GetImageStatus());
            std::cerr << "GetNextImage Error with FLIR ImageStatus=" + img_status << std::endl;
        }
        else
        {
            uint32_t xpad = image_ptr->GetXPadding();
            uint32_t ypad = image_ptr->GetYPadding();
            uint32_t width = image_ptr->GetWidth();
            uint32_t height = image_ptr->GetHeight();

            //convert yuv to bgr8
            flir::ImagePtr img_converted_ptr = image_ptr->Convert(flir::PixelFormatEnums::PixelFormat_BGR8);

            //image data contains padding. When allocating Mat container size, you need to account for the X,Y image data padding.
            cv::Mat tmp_mat = cv::Mat(height + ypad, width + xpad, CV_8UC3, img_converted_ptr->GetData(), img_converted_ptr->GetStride());
            tmp_mat.copyTo(_frame_color);
            tmp_mat.release();
            
            //release image buffers
            image_ptr->Release();
        }
    }
    
    const char* camera_flir_blackfly::flir_pixel_format_to_string(FLIR_PixelFormat flir_pixel_format)
    {
        switch(flir_pixel_format)
        {

            case FLIR_PixelFormat::PixelFormat_Mono8:
                return "Mono8";
                
            case FLIR_PixelFormat::PixelFormat_Mono16:
                return "Mono16";
                
            case FLIR_PixelFormat::PixelFormat_RGB8Packed:
                return "RGB8Packed";
                
            case FLIR_PixelFormat::PixelFormat_BayerGR8:
                return "BayerGR8";
                
            case FLIR_PixelFormat::PixelFormat_BayerRG8:
                return "BayerRG8";
                
            case FLIR_PixelFormat::PixelFormat_BayerGB8:
                return "BayerGB8";
                
            case FLIR_PixelFormat::PixelFormat_BayerBG8:
                return "BayerBG8";
                
            case FLIR_PixelFormat::PixelFormat_BayerGR16:
                return "BayerGR16";
                
            case FLIR_PixelFormat::PixelFormat_BayerRG16:
                return "BayerRG16";
                
            case FLIR_PixelFormat::PixelFormat_BayerGB16:
                return "BayerGB16";
                
            case FLIR_PixelFormat::PixelFormat_BayerBG16:
                return "BayerBG16";
                
            case FLIR_PixelFormat::PixelFormat_Mono12Packed:
                return "Mono12Packed";
                
            case FLIR_PixelFormat::PixelFormat_BayerGR12Packed:
                return "BayerGR12Packed";
                
            case FLIR_PixelFormat::PixelFormat_BayerRG12Packed:
                return "BayerRG12Packed";
                
            case FLIR_PixelFormat::PixelFormat_BayerGB12Packed:
                return "BayerGB12Packed";
                
            case FLIR_PixelFormat::PixelFormat_BayerBG12Packed:
                return "BayerBG12Packed";
                
            case FLIR_PixelFormat::PixelFormat_YUV411Packed:
                return "YUV411Packed";
                
            case FLIR_PixelFormat::PixelFormat_YUV422Packed:
                return "YUV422Packed";
                
            case FLIR_PixelFormat::PixelFormat_YUV444Packed:
                return "YUV444Packed";
                
            case FLIR_PixelFormat::PixelFormat_Mono12p:
                return "Mono12p";
                
            case FLIR_PixelFormat::PixelFormat_BayerGR12p:
                return "BayerGR12p";
                
            case FLIR_PixelFormat::PixelFormat_BayerRG12p:
                return "BayerRG12p";
                
            case FLIR_PixelFormat::PixelFormat_BayerGB12p:
                return "BayerGB12p";
                
            case FLIR_PixelFormat::PixelFormat_BayerBG12p:
                return "BayerBG12p";
                
            case FLIR_PixelFormat::PixelFormat_YCbCr8:
                return "YCbCr8";
                
            case FLIR_PixelFormat::PixelFormat_YCbCr422_8:
                return "YCbCr422_8";
                
            case FLIR_PixelFormat::PixelFormat_YCbCr411_8:
                return "YCbCr411_8";
                
            case FLIR_PixelFormat::PixelFormat_BGR8:
                return "BGR8";
                
            case FLIR_PixelFormat::PixelFormat_BGRa8:
                return "BGRa8";
                
        }

        return "UNKNOWN";
    }
}
#endif