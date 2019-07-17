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

#include <mutex>

#include "camera_flir_blackfly_system.hpp"

#ifdef TC_USE_BLACKFLY

namespace terraclear
{
    camera_flir_blackfly_system::camera_flir_blackfly_system() 
    {
                //defaults
        try
        {
            _mutex.lock();
                //smartptr to dlir System..
                _flir_system = flir::System::GetInstance();

                //get flir cameras attached to system
                _flir_camera_list = _flir_system->GetCameras();  
            _mutex.unlock();
        }
        catch (flir::Exception &e)
        {
            throw get_flir_error(e);
        } 
        
    }

    void camera_flir_blackfly_system::release()
    {
        if (!_disposing)
        {
            _disposing = true;

            //cleanup resources..
            try
            {
                //clear cameras before releasing system instance..
                _flir_camera_list.Clear();

                //release system
                _flir_system->ReleaseInstance();  
            }
            catch (flir::Exception &e)
            {
                std::cerr << "Flir System: " << e.GetErrorMessage() << " @ " << e.GetFunctionName() << std::endl;
            }
        }        
    }
     
    camera_flir_blackfly_system::~camera_flir_blackfly_system() 
    {
        release();
    }
    
    error_base camera_flir_blackfly_system::get_flir_error(flir::Exception &e)
    {
        std::stringstream strstrm;
        strstrm << FLIR_ERR_STR << e.GetErrorMessage() << ":" << e.GetFunctionName();
        return error_base(strstrm.str(), e.GetError());
    }
    
    error_base camera_flir_blackfly_system::get_generic_error(std::string err_string)
    {
        std::stringstream strstrm;
        strstrm << FLIR_ERR_STR << err_string;
        return error_base(strstrm.str());
    }    
    
    std::vector<std::string> camera_flir_blackfly_system::get_cameras()
    {
        std::vector<std::string> cam_list;
                
        try
        {
            //get all serials.
            _mutex.lock();
            uint32_t cam_count = _flir_camera_list.GetSize();
            _mutex.unlock();

            for (uint32_t cam_index = 0; cam_index < cam_count; cam_index++)
            {
                std::string cam_serial = _flir_camera_list.GetByIndex(cam_index)->GetUniqueID().c_str();
                cam_list.push_back(cam_serial);
            }

        }
        catch (flir::Exception &e)
        {
            throw get_flir_error(e);
        }      
        
        return cam_list;
    }    

    uint32_t camera_flir_blackfly_system::get_camera_count()
    {
        _mutex.lock();
            uint32_t list_size = _flir_camera_list.GetSize();
        _mutex.unlock();

        return list_size;
    }

    flir::CameraPtr camera_flir_blackfly_system::get_camera(std::string cam_serial)
    {
        _mutex.lock();
            flir::CameraPtr cam_ptr = _flir_camera_list.GetBySerial(cam_serial);
        _mutex.unlock();

        return cam_ptr;
    }
    
    flir::CameraPtr camera_flir_blackfly_system::get_camera(int cam_index)
    {
        _mutex.lock();
            flir::CameraPtr cam_ptr = _flir_camera_list.GetByIndex(cam_index);
        _mutex.unlock();

        return cam_ptr;
    }
}
#endif /* Conditional BLACKFLY support */