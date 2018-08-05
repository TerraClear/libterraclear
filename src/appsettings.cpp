/*
 * Basic App Settings from JSON file
 * Copyright (C) 2017 TerraClear, Inc.
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

    bool appsettings::getvalue_bool(std::string setting_name, bool default_value)
    {
        return _settings.get(setting_name, default_value).asBool();
    }

    int32_t appsettings::getvalue_int(std::string setting_name, int default_value)
    {
       return _settings.get(setting_name, default_value).asInt();
    }

    uint32_t appsettings::getvalue_uint(std::string setting_name, uint32_t default_value)
    {
       return _settings.get(setting_name, default_value).asUInt();
    }

    float appsettings::getvalue_float(std::string setting_name, float default_value)
    {
       return _settings.get(setting_name, default_value).asFloat();
    }

    std::string appsettings::getvalue_string(std::string setting_name, std::string default_value)
    {
        return _settings.get(setting_name, default_value).asString();
    }


    appsettings::~appsettings() 
    {
    }

}