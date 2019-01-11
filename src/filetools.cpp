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

    std::vector <std::string> filetools::sort_files(std::vector<std::string> file_list, bool ascending)
    {
        if (ascending)
            std::sort(file_list.begin(), file_list.end(), std::greater<std::string>());
        else
            std::sort(file_list.begin(), file_list.end(), std::less<std::string>());

        return file_list;
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
                    retval.push_back(path + "/" + entry_name);
            }
            closedir( dir_pointer );

            if (sort_by_name)
                std::sort( retval.begin(), retval.end() );
        }

        return retval;
    }

    std::string filetools::get_filename(std::string file_path)
    {
        //find the last "/"
        int path_pos = file_path.find_last_of("/");
        int path_substr = (path_pos >= 0) ? path_pos+1 : 0;


        return file_path.substr(path_substr, file_path.length() - path_pos);
    }

    std::string filetools::get_base_path(std::string file_path)
    {
        std::string base_path = file_path.substr(0, file_path.length() - get_filename(file_path).length());
        return base_path;
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

    //get file extension.
    std::string filetools::remove_extension(std::string file_name)
    {
        //get current extension
        std::string current_extension = get_extension(file_name);

        //generate new file name
        uint16_t str_end = (file_name.length() > current_extension.length()) ? file_name.length() - (current_extension.length()+1) : current_extension.length();
        std::string new_file_name = file_name.substr(0, str_end);

        return new_file_name;
    }
    
    //swap extenstion
    std::string filetools::replace_extension(std::string file_name, std::string new_extension)
    {
        //get current extension
        std::string current_extension = get_extension(file_name);

        //generate new file name
        std::string new_file_name = file_name;
        new_file_name.replace(file_name.length() - current_extension.length(), current_extension.length(), new_extension);

        return new_file_name;
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
                std::string file_ext = ext_list[i];
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

    //copy a file
    void filetools::copy_file(std::string file_source, std::string file_target, bool replace_file)
    {
        //check if exists and delete if replaced
        if (file_exists(file_target) && replace_file)
            std::remove(file_target.c_str()); // delete file
        
        std::ifstream  src(file_source, std::ios::binary);
        std::ofstream  dst(file_target, std::ios::binary);
        dst << src.rdbuf();        
        src.close();
        dst.close();
    }
    
    std::string filetools::path_append(std::string path_str, std::string append_str)
    {
        std::stringstream strstrm;

        //see if there is a trailing "/"
        std::string path_part = "";
        if (path_str.length() > 0)
        {
            int path_pos = path_str.find_last_of("/");
            int path_substr = (path_pos + 1 == path_str.size()) ? path_pos : path_str.size();
            path_part = (path_pos == 0) ? "" :  path_str.substr(0, path_substr);
        }

        //see if there is a leading "/"
        int append_pos = append_str.find_first_of("/");
        int append_substr = (append_pos == 0) ? 1 : 0;
        std::string append_part = append_str.substr(append_substr, append_str.length() - append_substr);

        strstrm << path_part << "/" << append_part ;

        return strstrm.str();        
    }

    //generates a file name to prevent duplicates...
    std::string filetools::generate_filename_seq(std::string file_name)
    {
        //get current extension
        std::string file_ext = get_extension(file_name);
        
        int file_seq = 1;
        int max_tries = 1000000;
        std::string default_file = file_name;
        std::string tmp_file = default_file;

        do 
        {
            std::stringstream strstrm;
            strstrm << remove_extension(file_name) <<  "_" << file_seq << "." << file_ext;
            tmp_file = strstrm.str();

            file_seq++;
            max_tries --;

            //max file sequence generation reached, use default and overwrite.
            if (max_tries <= 0)
            {
                tmp_file = default_file;
                break;
            }

        } while (file_exists(tmp_file));

        return tmp_file;
    }
    
    //read lines from text file..
    std::vector<std::string> filetools::read_lines(std::string txtfilepath)
    {
        std::vector<std::string> names_list;

        //If names file exist, load them into vector..
        if (file_exists(txtfilepath.c_str()))
        {
            // bounding boxes
            std::ifstream infile(txtfilepath.c_str());

            std::string class_name;
            //read all names from file 
            while (std::getline(infile, class_name))
            {
                names_list.push_back(class_name);
            }
        }//endif names file..

        return names_list;
    }    
}