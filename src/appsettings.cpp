/*
 * Basic App Settings from JSON file
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

#include "appsettings.hpp"

namespace terraclear
{
    

    appsettings::appsettings() 
    {
        initfile();
    }

    appsettings::appsettings(std::string json_filename)
    {
        _json_filename = json_filename;
        initfile();
    }

    void appsettings::initfile()
    {
        //read config values from json file..
        std::ifstream config_doc(_json_filename, std::ifstream::binary);
        config_doc >> _settings;
        config_doc.close();
    }
    
    Json::Value appsettings::getsection(std::string section_name)
    {
        Json::Value retval;
        
        if (section_name.size() > 0)
        {
            retval = _settings[section_name];
        }
        else
        {
            retval = _settings;
        }
        
        return retval;
    }

    std::vector<Json::Value> appsettings::getarray(std::string section_name, std::string array_name) 
    {
        std::vector<Json::Value> retval;

        Json::Value json_array = getsection(section_name);
        for (const Json::Value& array_entry : json_array[array_name]) 
        {
            retval.push_back(array_entry);   
        }

        return retval;
    } 
    
    bool appsettings::getvalue_bool(std::string setting_name, bool default_value)
    {
       
        return getvalue_bool("", setting_name, default_value);
    }
    
    bool appsettings::getvalue_bool(std::string section_name, std::string setting_name, bool default_value)
    {
       
        return getsection(section_name).get(setting_name, default_value).asBool();
    }
    
    std::vector<bool>  appsettings::getvalue_bool_array(std::string setting_name)
    {        
        return getvalue_bool_array("", setting_name);
    }
    
    std::vector<bool>  appsettings::getvalue_bool_array(std::string section_name, std::string setting_name)
    {
        std::vector<bool> retval;
        
        for (auto json_value : getarray(section_name, setting_name))
            retval.push_back(json_value.asBool());
        
        return retval;
    }
    
    
    
    int32_t appsettings::getvalue_int(std::string setting_name, int default_value)
    {
       return getvalue_int("", setting_name, default_value);
    }

    int32_t appsettings::getvalue_int(std::string section_name, std::string setting_name, int default_value)
    {
       return getsection(section_name).get(setting_name, default_value).asInt();
    }

    std::vector<int32_t> appsettings::getvalue_int_array(std::string setting_name)
    {        
        return getvalue_int_array("", setting_name);
    }
    
    std::vector<int32_t> appsettings::getvalue_int_array(std::string section_name, std::string setting_name)
    {
        std::vector<int32_t> retval;
        
        for (auto json_value : getarray(section_name, setting_name))
            retval.push_back(json_value.asInt());
        
        return retval;
    }

    
    
    uint32_t appsettings::getvalue_uint(std::string setting_name, uint32_t default_value)
    {
       return getvalue_uint("", setting_name, default_value);
    }

    uint32_t appsettings::getvalue_uint(std::string section_name, std::string setting_name, uint32_t default_value)
    {
       return getsection(section_name).get(setting_name, default_value).asUInt();
    }

    std::vector<uint32_t> appsettings::getvalue_uint_array(std::string setting_name)
    {        
        return getvalue_uint_array("", setting_name);
    }
    
    std::vector<uint32_t> appsettings::getvalue_uint_array(std::string section_name, std::string setting_name)
    {
        std::vector<uint32_t> retval;
        
        for (auto json_value : getarray(section_name, setting_name))
            retval.push_back(json_value.asUInt());
        
        return retval;
    }
    
    
    
    float appsettings::getvalue_float(std::string setting_name, float default_value)
    {
       return getvalue_float("", setting_name, default_value);
    }

    float appsettings::getvalue_float(std::string section_name, std::string setting_name, float default_value)
    {
       return getsection(section_name).get(setting_name, default_value).asFloat();
    }

    std::vector<float> appsettings::getvalue_float_array(std::string setting_name)
    {        
        return getvalue_float_array("", setting_name);
    }
    
    std::vector<float> appsettings::getvalue_float_array(std::string section_name, std::string setting_name)
    {
        std::vector<float> retval;
        
        for (auto json_value : getarray(section_name, setting_name))
            retval.push_back(json_value.asFloat());
        
        return retval;
    }
    
    
        
    double appsettings::getvalue_double(std::string setting_name, double default_value)
    {
       return getvalue_double("", setting_name, default_value);
    }

    double appsettings::getvalue_double(std::string section_name, std::string setting_name, double default_value)
    {
       return getsection(section_name).get(setting_name, default_value).asDouble();
    }

    std::vector<double> appsettings::getvalue_double_array(std::string setting_name)
    {        
        return getvalue_double_array("", setting_name);
    }
    
    std::vector<double> appsettings::getvalue_double_array(std::string section_name, std::string setting_name)
    {
        std::vector<double> retval;
        
        for (auto json_value : getarray(section_name, setting_name))
            retval.push_back(json_value.asDouble());
        
        return retval;
    }
    
    
        
    std::string appsettings::getvalue_string(std::string setting_name, std::string default_value)
    {
        return getvalue_string("", setting_name, default_value);
    }

    std::string appsettings::getvalue_string(std::string section_name, std::string setting_name, std::string default_value)
    {
        return getsection(section_name).get(setting_name, default_value).asString();
    }

    std::vector<std::string>  appsettings::getvalue_string_array(std::string setting_name)
    {        
        return getvalue_string_array("", setting_name);
    }
    
    std::vector<std::string>  appsettings::getvalue_string_array(std::string section_name, std::string setting_name)
    {
        std::vector<std::string> retval;
        
        for (auto json_value : getarray(section_name, setting_name))
            retval.push_back(json_value.asString());
        
        return retval;
    }
    
    
            
    appsettings::~appsettings() 
    {
    }

}