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

#ifndef APPSETTINGS_HPP
#define APPSETTINGS_HPP

#include <iostream>
#include <fstream>

//jsoncpp https://linux.tips/programming/how-to-install-and-use-json-cpp-library-on-ubuntu-linux-os
#ifdef __linux__
    #include <jsoncpp/json/json.h>
#else
    #include <json/json.h>
#endif


namespace terraclear
{
    class appsettings 
    {
        public:
            appsettings();
            appsettings(std::string json_filename);
            virtual ~appsettings();

            bool getvalue_bool(std::string setting_name, bool default_value = false);
            bool getvalue_bool(std::string section_name, std::string setting_name, bool default_value = false);
            std::vector<bool> getvalue_bool_array(std::string setting_name);
            std::vector<bool> getvalue_bool_array(std::string section_name, std::string setting_name);

            int32_t getvalue_int(std::string setting_name, int32_t default_value = 0);
            int32_t getvalue_int(std::string section_name, std::string setting_name, int32_t default_value = 0);
            std::vector<int32_t> getvalue_int_array(std::string setting_name);
            std::vector<int32_t> getvalue_int_array(std::string section_name, std::string setting_name);

            uint32_t getvalue_uint(std::string setting_name, uint32_t default_value = 0);
            uint32_t getvalue_uint(std::string section_name, std::string setting_name, uint32_t default_value = 0);
            std::vector<uint32_t> getvalue_uint_array(std::string setting_name);
            std::vector<uint32_t> getvalue_uint_array(std::string section_name, std::string setting_name);

            float getvalue_float(std::string setting_name, float default_value = 0.0f);
            float getvalue_float(std::string section_name, std::string setting_name, float default_value = 0.0f);
            std::vector<float> getvalue_float_array(std::string setting_name);
            std::vector<float> getvalue_float_array(std::string section_name, std::string setting_name);

            double getvalue_double(std::string setting_name, double default_value = 0.0f);
            double getvalue_double(std::string section_name, std::string setting_name, double default_value = 0.0f);
            std::vector<double> getvalue_double_array(std::string setting_name);
            std::vector<double> getvalue_double_array(std::string section_name, std::string setting_name);

            std::string getvalue_string(std::string setting_name, std::string default_value = "");           
            std::string getvalue_string(std::string section_name, std::string setting_name, std::string default_value);                 
            std::vector<std::string> getvalue_string_array(std::string setting_name);
            std::vector<std::string> getvalue_string_array(std::string section_name, std::string setting_name);
            
        private:
            Json::Value _settings;

            std::string _json_filename = "appsettings.json";
            void initfile();
            Json::Value getsection(std::string section_name);
            std::vector<Json::Value> getarray(std::string section_name, std::string array_name);


    };
}
#endif /* APPSETTINGS_HPP */

