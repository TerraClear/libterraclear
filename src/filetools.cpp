/*
 * Basic class for file & IO helpers 
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

#include "filetools.hpp"

namespace  terraclear
{
    
    filetools::filetools() 
    {
    }

    filetools::~filetools() 
    {
    }

    //check if file exists..
    bool  filetools::file_exists(const std::string filename) 
    {
      std::ifstream ifile(filename.c_str());
      return (bool)ifile;
    }

    //get all files in specific directory
    std::vector<std::string> filetools::read_directory( const std::string path, bool sort_by_name)
    {

//TODO: check errno is non zero, throw exception with detail..

        errno = 0;

        std::vector <std::string> retval;
        dirent* dir_entry;
        DIR* dir_pointer;
        dir_pointer = opendir( path.empty() ? "." : path.c_str() );

        if (dir_pointer)
        {
            while (true)
            {
                errno = 0;
                dir_entry = readdir( dir_pointer );

                if (dir_entry == NULL) 
                    break;

                std::string entry_name = std::string(dir_entry->d_name);
                //omit . and .. directories
                if ((entry_name != ".") && (entry_name != "..")) 
                    retval.push_back(entry_name);
            }
            closedir( dir_pointer );

            if (sort_by_name)
                std::sort( retval.begin(), retval.end() );
        }

        return retval;
    }


    //get file extension.
    std::string filetools::get_extension(std::string file_name)
    {
        std::string file_ext = "";

        int ext_pos = file_name.find_last_of(".");

        if ((ext_pos > 0) && (ext_pos < file_name.length()))
        {
            ext_pos ++;
            file_ext = file_name.substr(ext_pos, file_ext.length() - ext_pos);    
        }

        return file_ext;
    }

    std::vector<std::string> filetools::filter_files(std::vector<std::string> file_list, std::string file_ext, bool case_sensitive)
    {
        std::vector<std::string> ext_list;
        ext_list.push_back(file_ext);

       return filter_files(file_list, ext_list, case_sensitive);
    }
    
    std::vector <std::string> filetools::filter_files(std::vector<std::string> file_list, std::vector<std::string> ext_list, bool case_sensitive)
    {
        //convert all extensions in list to upper case
        if (!case_sensitive)
        {
            for (int i = 0; i < ext_list.size(); i++)
            {                
                //get file extension and convert to uppercase for non case sensitive compare.
                std::string file_ext = get_extension(ext_list[i]);
                std::transform(file_ext.begin(), file_ext.end(), file_ext.begin(), ::toupper); 
                ext_list[i] = file_ext;                
            }            
        }

        //remove all entries that don't end in specific extensions.
        std::vector <std::string> retval;
        for (auto entry_name : file_list)
        {
            std::string file_ext = get_extension(entry_name);

            //get file extension and convert to uppercase for non case sensitive compare.
            if (!case_sensitive)
            {
                std::transform(file_ext.begin(), file_ext.end(), file_ext.begin(), ::toupper); 
            }

            //only retain files with specific ext that were supplied.
            if (std::find(ext_list.begin(), ext_list.end(), file_ext) != ext_list.end())
            {
                    retval.push_back(entry_name);
            }

        }

        return retval;
    }

}