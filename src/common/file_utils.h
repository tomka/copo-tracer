#ifndef FILE_UTILS_H
#define FILE_UTILS_H
#include <string>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <direct.h>

inline std::string get_file_ending(const std::string& filename)
{
	std::string::size_type pos = filename.find_last_of(".");
	if(pos == std::string::npos)
		return "";
	if(pos+1 == filename.size())
		return "";

	return filename.substr(pos+1);
}


inline std::string get_path_name(const std::string& filename)
{
	std::string::size_type pos1 = filename.find_last_of("/");
	std::string::size_type pos2 = filename.find_last_of("\\");
	std::string::size_type pos=std::string::npos;
	if(pos1 != std::string::npos)
	{
		pos = pos1;
		if(pos2 != std::string::npos && pos2 > pos)
			pos = pos2;
	}
	else
	{
		if(pos2 != std::string::npos )
			pos = pos2;
	}
	if(pos == std::string::npos)
		return std::string("");
	

	return filename.substr(0,pos+1);
}

inline std::string get_file_name_without_ending(const std::string& filename)
{
	std::string::size_type pos = filename.find_last_of(".");
	if(pos == std::string::npos)
		return filename;
	if(pos-1 ==0)
		return "";

	return filename.substr(0,pos-1);
}


inline std::string get_numbered_filename(const std::string& name,int number,const std::string& ending,int field=3)
{
	std::stringstream ss;
	ss<< name<<std::setw(field) << std::setfill('0') << number <<"."<<ending;
	return ss.str();

}



inline bool file_exists(const std::string& filename)
{
	std::ifstream file(filename.c_str());
	
	if( file.is_open() ) 
	{
		file.close();
		return true;
	} else 
	{
		return false;
	}

}


 
void mkdir(const std::string& foldername)
{
	 _mkdir(foldername.c_str());
}

 
void rmdir(const std::string& foldername)
{
	 _rmdir(foldername.c_str());
}


#endif