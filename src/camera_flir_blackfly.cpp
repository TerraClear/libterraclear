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
#include <spinnaker/Camera.h>

#include "camera_flir_blackfly.hpp"

#ifdef TC_USE_BLACKFLY

namespace terraclear
{  

    camera_flir_blackfly::camera_flir_blackfly()
    {
        //defaults
        _flir_reverseY = true;
        _flir_pixel_format = FLIR_PixelFormat::PixelFormat_YUV411Packed;
        
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
                            ptrReverseY->SetValue(true);
                        }

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
                                    throw get_generic_error("Could verify '" + pixel_format_string + "' PixelFormat value is readable.");
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
            
            //copy to base camera frame
            tmp_mat.copyTo(_frame_color);
            
            //release image buffers
            image_ptr->Release();
        }
    }
    
    std::string camera_flir_blackfly::flir_pixel_format_to_string(FLIR_PixelFormat flir_pixel_format)
    {
        std::string retval = "UNKNOWN";
        
        switch(flir_pixel_format)
        {

            case FLIR_PixelFormat::PixelFormat_Mono8:
                retval = "Mono8";
                break;
            case FLIR_PixelFormat::PixelFormat_Mono16:
                retval = "Mono16";
                break;
            case FLIR_PixelFormat::PixelFormat_RGB8Packed:
                retval = "RGB8Packed";
                break;
            case FLIR_PixelFormat::PixelFormat_BayerGR8:
                retval = "BayerGR8";
                break;
            case FLIR_PixelFormat::PixelFormat_BayerRG8:
                retval = "BayerRG8";
                break;
            case FLIR_PixelFormat::PixelFormat_BayerGB8:
                retval = "BayerGB8";
                break;
            case FLIR_PixelFormat::PixelFormat_BayerBG8:
                retval = "BayerBG8";
                break;
            case FLIR_PixelFormat::PixelFormat_BayerGR16:
                retval = "BayerGR16";
                break;
            case FLIR_PixelFormat::PixelFormat_BayerRG16:
                retval = "BayerRG16";
                break;
            case FLIR_PixelFormat::PixelFormat_BayerGB16:
                retval = "BayerGB16";
                break;
            case FLIR_PixelFormat::PixelFormat_BayerBG16:
                retval = "BayerBG16";
                break;
            case FLIR_PixelFormat::PixelFormat_Mono12Packed:
                retval = "Mono12Packed";
                break;
            case FLIR_PixelFormat::PixelFormat_BayerGR12Packed:
                retval = "BayerGR12Packed";
                break;
            case FLIR_PixelFormat::PixelFormat_BayerRG12Packed:
                retval = "BayerRG12Packed";
                break;
            case FLIR_PixelFormat::PixelFormat_BayerGB12Packed:
                retval = "BayerGB12Packed";
                break;
            case FLIR_PixelFormat::PixelFormat_BayerBG12Packed:
                retval = "BayerBG12Packed";
                break;
            case FLIR_PixelFormat::PixelFormat_YUV411Packed:
                retval = "YUV411Packed";
                break;
            case FLIR_PixelFormat::PixelFormat_YUV422Packed:
                retval = "YUV422Packed";
                break;
            case FLIR_PixelFormat::PixelFormat_YUV444Packed:
                retval = "YUV444Packed";
                break;
            case FLIR_PixelFormat::PixelFormat_Mono12p:
                retval = "Mono12p";
                break;
            case FLIR_PixelFormat::PixelFormat_BayerGR12p:
                retval = "BayerGR12p";
                break;
            case FLIR_PixelFormat::PixelFormat_BayerRG12p:
                retval = "BayerRG12p";
                break;
            case FLIR_PixelFormat::PixelFormat_BayerGB12p:
                retval = "BayerGB12p";
                break;
            case FLIR_PixelFormat::PixelFormat_BayerBG12p:
                retval = "BayerBG12p";
                break;
            case FLIR_PixelFormat::PixelFormat_YCbCr8:
                retval = "YCbCr8";
                break;
            case FLIR_PixelFormat::PixelFormat_YCbCr422_8:
                retval = "YCbCr422_8";
                break;
            case FLIR_PixelFormat::PixelFormat_YCbCr411_8:
                retval = "YCbCr411_8";
                break;
            case FLIR_PixelFormat::PixelFormat_BGR8:
                retval = "BGR8";
                break;
            case FLIR_PixelFormat::PixelFormat_BGRa8:
                retval = "BGRa8";
                break;
        }

        return retval;
    }
}

#endif
