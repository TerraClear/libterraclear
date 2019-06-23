/*
 * Basic class for file & IO helpers 
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

#include <cstdlib>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <fstream>
#include <dirent.h>
#include <sstream> //stringstream


#ifndef FILETOOLS_HPP
#define FILETOOLS_HPP

namespace  terraclear
{

    class filetools 
    {
        public:
            filetools();
            virtual ~filetools();
            
            //static helper functions
            static bool         file_exists(const std::string  filename);
            static std::string  get_filename(std::string file_path);
            static std::string  get_base_path(std::string file_path);
            static std::string  get_extension(std::string file_name);
            static std::string  remove_extension(std::string file_name);
            static std::string  replace_extension(std::string file_name, std::string new_extension);
            static std::string  path_append(std::string path_str, std::string append_str);
            static std::string  generate_filename_seq(std::string file_name, std::string extra = "");
            
            static std::vector <std::string>    read_directory( const std::string path, bool sort_by_name = true);
            static std::vector <std::string>    sort_files(std::vector<std::string> file_list, bool ascending = true);
            static std::vector <std::string>    filter_files(std::vector<std::string> file_list, std::string filex_ext, bool case_sensitive = true);
            static std::vector <std::string>    filter_files(std::vector<std::string> file_list, std::vector<std::string> ext_list, bool case_sensitive = true);

            static void copy_file(std::string file_source, std::string file_target, bool replace_file = true);
            
            static std::vector<std::string>     read_lines(std::string txtfilepath);
            
    
        private:
    };

}
#endif /* FILETOOLS_HPP */

