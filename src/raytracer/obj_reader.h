#pragma once

#include <vector>
#include <string>
#include <map>
#include "tiny_vec.h"
#include <fstream>
#include <sstream>
#include <limits>
#include "progress.h"



struct obj_polygon
{
	std::vector<unsigned> vertex_indices; 
	std::vector<unsigned> normal_indices; 
	std::vector<unsigned> texcoord_indices; 
	
	
	obj_polygon(unsigned n)
	{
		vertex_indices.resize(n);
		
		
	}

};



struct obj_reader
{

	std::vector<tiny_vec<float,3> > vertices; 
    std::vector<tiny_vec<float,3> > normals; 
    std::vector<tiny_vec<float,2> > texcoords; 
    std::vector<obj_polygon> faces; 


	void rescale(tiny_vec<float,3>& minv, tiny_vec<float,3>& maxv, bool uniform_scale=true)
	{
		tiny_vec<float,3> lo(std::numeric_limits<float>::infinity(),std::numeric_limits<float>::infinity(),std::numeric_limits<float>::infinity());
		tiny_vec<float,3> hi(-std::numeric_limits<float>::infinity(),-std::numeric_limits<float>::infinity(),-std::numeric_limits<float>::infinity());
		
		for(std::vector<tiny_vec<float,3> >::iterator it = vertices.begin(); it != vertices.end(); ++it)
		{
			for(int i = 0; i <3; i++)
			{
				lo[i] = (std::min)((*it)[i],lo[i]);
				hi[i] = (std::max)((*it)[i],hi[i]);
			}
		}

		
		float s1 = (maxv[0] - minv[0])/ (hi[0]-lo[0]);
		float s2 = (maxv[1] - minv[1])/ (hi[1]-lo[1]);
		float s3 = (maxv[2] - minv[2])/ (hi[2]-lo[2]);
		if(uniform_scale)
			s1=s3=s2;


		float t1 = 0.5f*(minv[0]+maxv[0]) - s1*0.5f*(lo[0]+hi[0]);
		float t2 = 0.5f*(minv[1]+maxv[1]) - s2*0.5f*(lo[1]+hi[1]);
		float t3 = 0.5f*(minv[2]+maxv[2]) - s3*0.5f*(lo[2]+hi[2]);

		for(std::vector<tiny_vec<float,3> >::iterator it = vertices.begin(); it != vertices.end(); ++it)
		{
			
				(*it)[0] = s1*(*it)[0]+t1;
				(*it)[1] = s2*(*it)[1]+t2;
				(*it)[2] = s3*(*it)[2]+t3;
				
		}


	}
	/*void flip(float x, float y, float z)
	{
		for(std::vector<tiny_vec<float,3> >::iterator it = vertices.begin(); it != vertices.end(); ++it)
		{
			
				(*it)[0] = x*(*it)[0];
				(*it)[1] = y*(*it)[1];
				(*it)[2] = z*(*it)[2];
				
		}

	}*/

	void tokenize(const std::string& str,const std::string& delimiters,std::vector<std::string>& tokens)
	{
		tokens.clear();

    	
		// skip delimiters at beginning.
		std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    	
		// find first "non-delimiter".
		std::string::size_type pos = str.find_first_of(delimiters, lastPos);

		while (std::string::npos != pos || std::string::npos != lastPos)
    	{
        	// found a token, add it to the vector.
        	tokens.push_back(str.substr(lastPos, pos - lastPos));
		
        	// skip delimiters.  Note the "not_of"
        	lastPos = str.find_first_not_of(delimiters, pos);
		
        	// find next "non-delimiter"
        	pos = str.find_first_of(delimiters, lastPos);
    	}
	}

	float str_2_float(const std::string& str)
	{
		float d;
		std::stringstream ss(str);
		ss >> d;
		return d;

	}

	

	int str_2_int(const std::string& str)
	{
		int i;
		std::stringstream ss(str);
		ss >> i;
		return i;
	}


	void show_stats()
	{
		std::cout << "num vertices "<<vertices.size()<<std::endl;
		std::cout << "num normals "<<normals.size()<<std::endl;
		std::cout << "num texcoords "<<texcoords.size()<<std::endl;
		std::cout << "num faces "<<faces.size()<<std::endl;
	}
	bool read_obj(const std::string& filename)
	{
		std::streamoff filesize=0;
		std::fstream in(filename.c_str(), std::ios::in);
		
		if(in.is_open())
		{
			in.seekg(0,std::ios::end);
			filesize = in.tellg();
			in.seekg(0,std::ios::beg);
		
			
			std::string line;
			std::vector<std::string> tokens;
			vertices.clear();
			normals.clear();
			texcoords.clear(); 
			faces.clear(); 
				
		
			std::string::size_type pos;
			

			while(std::getline(in,line,'\n'))
			{
				progress((float) in.tellg(),(float)filesize);
					

				pos = line.find(std::string("#"));
				if(std::string::npos != pos)
				{
					continue;
				}
				
				pos = line.find(std::string("v "));
				if(std::string::npos != pos )
				{
					read_vertex(line);
					continue;
				}

				pos = line.find("vn ");
				if(std::string::npos != pos )
				{
					read_normal(line);
					continue;
				}

				pos = line.find("vt ");
				if(std::string::npos != pos )
				{
					read_texcoord(line);
					continue;
				}
				
					
				pos = line.find("f ");
				if(std::string::npos != pos)
				{
					read_polygon(line);
					continue;
				}

									
				
			}
			in.close();
			std::cout << "\r                      \n";
			return true;
		}
		std::cout << "error reading file "<<filename<<std::endl;
		return false;
	}


	void read_vertex(const std::string& line)
	{
		std::vector<std::string> tokens;
		tokenize(line," ",tokens);
		
		tiny_vec<float,3> v;
		v(0) = str_2_float(tokens[1]);
		v(1) = str_2_float(tokens[2]);
		v(2) = str_2_float(tokens[3]);
		vertices.push_back(v);
	}

	void read_normal(const std::string& line)
	{
		std::vector<std::string> tokens;
		tokenize(line," ",tokens);

		tiny_vec<float,3> v;
		v(0) = str_2_float(tokens[1]);
		v(1) = str_2_float(tokens[2]);
		v(2) = str_2_float(tokens[3]);
		normals.push_back(v);
	}

	void read_texcoord(const std::string& line)
	{
		std::vector<std::string> tokens;
		tokenize(line," ",tokens);
		tiny_vec<float,2> v;
		v(0) = str_2_float(tokens[1]);
		v(1) = str_2_float(tokens[2]);
		texcoords.push_back(v);
	}

	
	void read_polygon(const std::string& line)
	{
		std::vector<std::string> tokens;
		tokenize(line," ",tokens);
		
		obj_polygon poly(tokens.size() - 1);
		

		for(unsigned i = 1; i < tokens.size(); i++)
		{ 
			std::vector<std::string> smaller_tokens;
			tokenize(tokens[i], "//",smaller_tokens);
			poly.vertex_indices[i - 1] = str_2_int(smaller_tokens[0])-1;
     
			//if texture is not specified but normal
			if(smaller_tokens.size() == 2)
			{
				

				poly.normal_indices.push_back(str_2_int(smaller_tokens[1])-1); 
			}
			
      
			//if texture and normals are specified 
			if(smaller_tokens.size() == 3)
			{
				poly.texcoord_indices.push_back(str_2_int(smaller_tokens[1])-1); 
				poly.normal_indices.push_back(str_2_int(smaller_tokens[2])-1); 
				 
			}
      } 
	  faces.push_back(poly); 
	 
	}

};





