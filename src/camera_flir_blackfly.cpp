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
#include <spinnaker/SpinGenApi/Pointer.h>

#include "camera_flir_blackfly.hpp"

//#ifdef TC_USE_BLACKFLY

namespace terraclear
{  

    camera_flir_blackfly::camera_flir_blackfly()
    {
        //defaults
        _flir_reverseY = false;
        _flir_pixel_format = FLIR_PixelFormat::PixelFormat_BayerRG8;
        
        //init flir system
        init_flir_system();
        
        //init and start default camera 0
        init_camera(0); 
        
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
        strstrm << _base_errstr << e.GetErrorMessage() << ":" << e.GetFunctionName();
        return error_base(strstrm.str(), e.GetError());
    }
    
    error_base camera_flir_blackfly::get_generic_error(std::string err_string)
    {
        std::stringstream strstrm;
        strstrm << _base_errstr << err_string;
        return error_base(strstrm.str());
    }
    
    void camera_flir_blackfly::init_flir_system()
    {
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
    
    void camera_flir_blackfly::init_camera(uint32_t camera_index)
    {
        if (get_camera_count() < camera_index)
        {
            throw get_generic_error("Camera Index Out Of Range");
        }
        else
        {
            try
            {
                //get camera ptr at index 0
                _flir_cam = _flir_camera_list.GetByIndex(camera_index);

                //init camera
                _flir_cam->Init();
                
                //get nodemaps
                //flir_api::INodeMap& nmTL = pcam->GetTLDeviceNodeMap();
                flir_api::INodeMap& flir_nodemap = _flir_cam->GetNodeMap();
                
                //get camera in AcquisitionMode mode enumerator
                flir_api::CEnumerationPtr penum_acquisition = flir_nodemap.GetNode("AcquisitionMode");

                //check in acquisition mode enumerator available and writable..
                if (!flir_api::IsAvailable(penum_acquisition) || (!flir_api::IsWritable(penum_acquisition)))
                {
                    throw get_generic_error("Could verify 'Acquisition' node entry is writable.");
                }
                else
                {
                    //get continuous capture mode entry.
                    flir_api::CEnumEntryPtr pentry_continuous = penum_acquisition->GetEntryByName("Continuous");

                    //check continuous entry is available and readable.
                    if (!flir_api::IsAvailable(pentry_continuous) || (!flir_api::IsReadable(pentry_continuous)))
                    {
                        throw get_generic_error("Could verify 'Continuous' Acquisition value is readable.");
                    }
                    else
                    {

                        // read integer value from entry node
                        int64_t value_continuous = pentry_continuous->GetValue();

                        // Set 'Continuous' value for Acquisition from on acquisition mode
                        penum_acquisition->SetIntValue(value_continuous);    

                        //get ptr to FlipY node item
                        flir_api::CBooleanPtr ptrReverseY = flir_nodemap.GetNode("ReverseY");

                        if (!flir_api::IsAvailable(ptrReverseY) || (!flir_api::IsWritable(ptrReverseY)))
                        {
                            throw get_generic_error("Could verify 'ReverseY' entry is writable.");
                        }
                        else
                        {
                            //setting reverseY
                            ptrReverseY->SetValue(_flir_reverseY);
                        }

                        //Set Exposure Mode to Manual exposure
                        flir_api::CEnumerationPtr exposureAuto = flir_nodemap.GetNode("ExposureAuto");
                        exposureAuto->SetIntValue(exposureAuto->GetEntryByName("Off")->GetValue());

                        //Set exposure time..
                        flir_api::CFloatPtr exposureTime = flir_nodemap.GetNode("ExposureTime");
                        exposureTime->SetValue(5000);
                        

                        //set Image Width/Height
                        flir_api::CIntegerPtr ptrWidth = flir_nodemap.GetNode("Width");
                        ptrWidth->SetValue( (uint64) FLIR_WIDTH/BIN_SIZE);
                        flir_api::CIntegerPtr ptrHeight = flir_nodemap.GetNode("Height");
                        ptrHeight->SetValue((uint64) FLIR_HEIGHT/BIN_SIZE);

                        //Enable Binning
                        flir_api::CIntegerPtr ptrBinH = flir_nodemap.GetNode("BinningHorizontal");
                        ptrBinH->SetValue(BIN_SIZE);
                        flir_api::CIntegerPtr ptrBinW = flir_nodemap.GetNode("BinningVertical");
                        ptrBinW->SetValue(BIN_SIZE);

                        //Force Frame Rate
                        flir_api::CBooleanPtr ptrFPSEnable = flir_nodemap.GetNode("AcquisitionFrameRateEnable");
                        ptrFPSEnable->SetValue(true);
                        flir_api::CFloatPtr ptrFPS = flir_nodemap.GetNode("AcquisitionFrameRate");
                        ptrFPS->SetValue(125);
                        
                        
                        //get pixel format and change if needed..
                        flir::PixelFormatEnums flir_format = _flir_cam->PixelFormat.GetValue();
                        if (flir_format != _flir_pixel_format)
                        {
                            //get ptr to pixel format node item
                            flir_api::CEnumerationPtr ptrPixelFormat = flir_nodemap.GetNode("PixelFormat");
                            if (!flir_api::IsAvailable(ptrPixelFormat) || (!flir_api::IsWritable(ptrPixelFormat)))
                            {
                                throw get_generic_error("Could verify 'PixelFormat' entry is writable.");
                            }
                            else
                            {
                                //format string..
                                std::string pixel_format_string = flir_pixel_format_to_string(_flir_pixel_format);

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
                                
                            }// end pixelform entry writable
                            
                        }// end pixelformat change check

                        // begin acquisition.
                        _flir_cam->BeginAcquisition();
                        
                    }// end Continuous Acquisition mode value readable   
                    
                }// end Acquisition mode entry writable
            
            }
            catch (flir::Exception &e)
            {
                throw get_flir_error(e);
            }
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
            std::cout << "GetNextImage Error with FLIR ImageStatus=" + img_status << std::endl;
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

//#endif
