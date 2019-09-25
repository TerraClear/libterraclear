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
#define TC_USE_BLACKFLY
#include "camera_flir_blackfly.hpp"

#ifdef TC_USE_BLACKFLY

namespace terraclear
{  
    camera_flir_blackfly::camera_flir_blackfly(camera_flir_blackfly_system* flir_system_ptr, flir_settings cam_settings)
    {        
        _cam_settings = cam_settings;
        
        //set camera to default..
        _flir_cam = flir_system_ptr->get_camera(0);
        init_camera(); 
    }

    camera_flir_blackfly::camera_flir_blackfly(camera_flir_blackfly_system* flir_system_ptr, flir_settings cam_settings, uint32_t cam_index)
    {
        _cam_settings = cam_settings;
        
        //set camera to specific index..
        _flir_cam = flir_system_ptr->get_camera(cam_index);
        init_camera(); 
        
    }
    
    camera_flir_blackfly::camera_flir_blackfly(camera_flir_blackfly_system* flir_system_ptr, flir_settings cam_settings, std::string cam_serial)
    {
        _cam_settings = cam_settings;

        //set camera to specific serial..
        _flir_cam = flir_system_ptr->get_camera(cam_serial);
        init_camera(); 
        
        //validate serial number
        if (_cam_serial.compare(cam_serial) != 0)
            throw camera_flir_blackfly_system::get_generic_error("FLIR Camera with Serial " + cam_serial + " not found!");
    }

    void camera_flir_blackfly::release()
    {
        if (!_disposing)
        {
            _disposing = true;
            
            //stop & cleanup camera resources..
            try
            {
                _flir_cam->EndAcquisition();
                _flir_cam->DeInit();

                //de-ref camera..
                _flir_cam = nullptr;
            }
            catch (flir::Exception &e)
            {
                std::cerr << _cam_serial << ": " << e.GetErrorMessage() << " @ " << e.GetFunctionName() << std::endl;
            }
        }        
    }
            
    camera_flir_blackfly::~camera_flir_blackfly() 
    {
        release();
    }

    void camera_flir_blackfly::init_camera()
    {
        
        try
        {
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

            //Set Exposure Mode (Values are "Off", "Continuous", "Once")
            flir_api::CEnumerationPtr pExposureAuto = flir_nodemap.GetNode("ExposureAuto");
            int exposure_val = pExposureAuto->GetEntryByName("Continuous")->GetValue();

            if (_cam_settings.exposure_auto)
            {
                pExposureAuto->SetIntValue(exposure_val);

            }
            else
            {
                exposure_val = pExposureAuto->GetEntryByName("Off")->GetValue();
                pExposureAuto->SetIntValue(exposure_val);

                //Set exposure time..
                flir_api::CFloatPtr pExposureTime = flir_nodemap.GetNode("ExposureTime");
                pExposureTime->SetValue(_cam_settings.exposure_time);
            }

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

            //force DeviceLinkThroughputLimit
            flir_api::CIntegerPtr ptrDeviceLinkThroughputLimit = flir_nodemap.GetNode("DeviceLinkThroughputLimit");
            if (IsAvailable(ptrDeviceLinkThroughputLimit) && IsWritable(ptrDeviceLinkThroughputLimit))
                ptrDeviceLinkThroughputLimit->SetValue(_cam_settings.device_link_limit);
            
            //set buffer size
            flir_api::CIntegerPtr ptrStreamBufferCount = flir_nodemap.GetNode("StreamDefaultBufferCount");
            if (IsAvailable(ptrStreamBufferCount) && IsWritable(ptrStreamBufferCount))
                ptrStreamBufferCount->SetValue(_cam_settings.buffer_size);            

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
                    throw camera_flir_blackfly_system::get_generic_error("PixelFormat '" + pixel_format_string + "' not available.");
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
            throw camera_flir_blackfly_system::get_flir_error(e);
        }          
    }
    
    std::string camera_flir_blackfly::get_serial()
    {
        return _cam_serial;
    }
    
    bool camera_flir_blackfly::frame_update()
    {   
        bool success = false;
        
        try
        {
            //acquire image
            flir::ImagePtr image_ptr = _flir_cam->GetNextImage();

            //check if image was complete on grab..
            if (image_ptr->GetImageStatus() !=  flir::ImageStatus::IMAGE_NO_ERROR) 
            {
                std::stringstream strstrm;
                strstrm << "GetNextImage Error with FLIR ImageStatus=" << flir_imgstatus_to_string(image_ptr->GetImageStatus());
                _last_error = strstrm.str();
            }
            else if (image_ptr->IsIncomplete())
            {
                _last_error = "GetNextImage Error with FLIR ImageStatus=INCOMPLETE";
            }
            else
            {
                uint32_t xpad = image_ptr->GetXPadding();
                uint32_t ypad = image_ptr->GetYPadding();
                uint32_t width = image_ptr->GetWidth();
                uint32_t height = image_ptr->GetHeight();

                //convert bgr8
                flir::ImagePtr img_converted_ptr = image_ptr->Convert(flir::PixelFormatEnums::PixelFormat_BGR8);

                //image data contains padding. When allocating Mat container size, you need to account for the X,Y image data padding.
                _buffer_camera = cv::Mat(height + ypad, width + xpad, CV_8UC3, img_converted_ptr->GetData(), img_converted_ptr->GetStride());

    // <<<<<<   FLIR DRIVER BUG WORK-AROUND TO SKIP BLANK OR PARTIAL BLANK FRAMES RETURNED BY API....
                //check if rows are blank.
                cv::Scalar sum_channels = cv::sum(_buffer_camera(cv::Rect(0, _buffer_camera.rows-3, _buffer_camera.cols, 2)));
                if (sum_channels[0] + sum_channels[1] +  sum_channels[2] > 0)
                {
                    mutex_lock();
                        _buffer_camera.copyTo(_frame_color);
                        success = true;
                    mutex_unlock();
                }
                else
                {
                    _last_error = "FLIR ImageStatus=BLANK";
                }
            }

            //release image buffers
            image_ptr->Release();            
        }
        catch (flir::Exception &e)
        {
            std::cerr << _cam_serial << ": " << e.GetErrorMessage() << " @ " << e.GetFunctionName() << std::endl;
        }        

        return success;
    }
    
    void camera_flir_blackfly::change_white_balance(float red_level, float blue_level)
    {
        Spinnaker::GenApi::INodeMap & nodeMap = _flir_cam->GetNodeMap();
        
        Spinnaker::GenApi::CEnumerationPtr balanceWhiteAuto = nodeMap.GetNode("BalanceWhiteAuto");
        balanceWhiteAuto->SetIntValue(balanceWhiteAuto->GetEntryByName("Off")->GetValue());

        Spinnaker::GenApi::CEnumerationPtr balanceRatioSelector = nodeMap.GetNode("BalanceRatioSelector");

        Spinnaker::GenApi::CFloatPtr balanceRatio = nodeMap.GetNode("BalanceRatio");
//        balanceRatio->SetValue(level);

        balanceRatioSelector->SetIntValue(balanceRatioSelector->GetEntryByName("Red")->GetValue());
        balanceRatio->SetValue(red_level);
        
//        balanceRatioSelector->SetIntValue(balanceRatioSelector->GetEntryByName("Green")->GetValue());
//        balanceRatio->SetValue(green_level);
        
        balanceRatioSelector->SetIntValue(balanceRatioSelector->GetEntryByName("Blue")->GetValue());
        balanceRatio->SetValue(blue_level);
    }
    
    void camera_flir_blackfly::change_gain(float gain_level)
    {
        Spinnaker::GenApi::INodeMap & nodeMap = _flir_cam->GetNodeMap();
        
        Spinnaker::GenApi::CEnumerationPtr gainAuto = nodeMap.GetNode("GainAuto");
        gainAuto->SetIntValue(gainAuto->GetEntryByName("Off")->GetValue());

        Spinnaker::GenApi::CFloatPtr gainValue = nodeMap.GetNode("Gain");
        gainValue->SetValue(gain_level);
    }
    
    void camera_flir_blackfly::change_exposure_time(int time)
    {
        Spinnaker::GenApi::INodeMap & nodeMap = _flir_cam->GetNodeMap();
        
        Spinnaker::GenApi::CEnumerationPtr exposureAuto = nodeMap.GetNode("ExposureAuto");
        exposureAuto->SetIntValue(exposureAuto->GetEntryByName("Off")->GetValue());

        Spinnaker::GenApi::CEnumerationPtr exposureMode = nodeMap.GetNode("ExposureMode");
        exposureMode->SetIntValue(exposureMode->GetEntryByName("Timed")->GetValue());

        Spinnaker::GenApi::CFloatPtr exposureTime = nodeMap.GetNode("ExposureTime");
        exposureTime->SetValue(time);
    }
    
    void camera_flir_blackfly::change_black_level(float level)
    {
        Spinnaker::GenApi::INodeMap & nodeMap = _flir_cam->GetNodeMap();
        std::cout << 1 << std::endl;
        
        Spinnaker::GenApi::CBooleanPtr blackLevelEnabled = nodeMap.GetNode("BlackLevelEnabled");
        std::cout << 2 << std::endl;
//        std::cout << blackLevelEnabled << std::endl;
        blackLevelEnabled->SetValue("True");
        std::cout << 3 << std::endl;

        Spinnaker::GenApi::CFloatPtr blackLevel = nodeMap.GetNode("BlackLevel");
        std::cout << 4 << std::endl;
        blackLevel->SetValue(level);
    }
    
    void camera_flir_blackfly::change_gamma(float gamma)
    {
        Spinnaker::GenApi::INodeMap & nodeMap = _flir_cam->GetNodeMap();
        
        Spinnaker::GenApi::CFloatPtr gam = nodeMap.GetNode("Gamma");
        gam->SetValue(gamma);
    }
    
    std::string camera_flir_blackfly::flir_pixel_format_to_string(FLIR_PixelFormat flir_pixel_format)
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
    
    std::string camera_flir_blackfly::flir_imgstatus_to_string(FLIR_ImageStatus flir_image_status)
    {
        switch(flir_image_status)
        {

            case FLIR_ImageStatus::IMAGE_UNKNOWN_ERROR:
                return "IMAGE_UNKNOWN_ERROR";
                
            case FLIR_ImageStatus::IMAGE_NO_ERROR:
                return "IMAGE_NO_ERROR";
                
            case FLIR_ImageStatus::IMAGE_CRC_CHECK_FAILED:
                return "IMAGE_CRC_CHECK_FAILED";
                
            case FLIR_ImageStatus::IMAGE_DATA_OVERFLOW:
                return "IMAGE_DATA_OVERFLOW";
                
            case FLIR_ImageStatus::IMAGE_MISSING_PACKETS:
                return "IMAGE_MISSING_PACKETS";
                
            case FLIR_ImageStatus::IMAGE_LEADER_BUFFER_SIZE_INCONSISTENT:
                return "IMAGE_LEADER_BUFFER_SIZE_INCONSISTENT";
                
            case FLIR_ImageStatus::IMAGE_TRAILER_BUFFER_SIZE_INCONSISTENT:
                return "IMAGE_TRAILER_BUFFER_SIZE_INCONSISTENT";
                
            case FLIR_ImageStatus::IMAGE_PACKETID_INCONSISTENT:
                return "IMAGE_PACKETID_INCONSISTENT";
                
            case FLIR_ImageStatus::IMAGE_MISSING_LEADER:
                return "IMAGE_MISSING_LEADER";
                
            case FLIR_ImageStatus::IMAGE_MISSING_TRAILER:
                return "IMAGE_MISSING_TRAILER";
                
            case FLIR_ImageStatus::IMAGE_DATA_INCOMPLETE:
                return "IMAGE_DATA_INCOMPLETE";
                
            case FLIR_ImageStatus::IMAGE_INFO_INCONSISTENT:
                return "IMAGE_INFO_INCONSISTENT";
                                       
            case FLIR_ImageStatus::IMAGE_CHUNK_DATA_INVALID:
                return "IMAGE_CHUNK_DATA_INVALID";
                                        
            case FLIR_ImageStatus::IMAGE_NO_SYSTEM_RESOURCES:
                return "IMAGE_NO_SYSTEM_RESOURCES";
                                
        }

        return "UNKNOWN";
        
    }
    
}
#endif