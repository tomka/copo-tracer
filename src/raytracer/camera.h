#ifndef CAMERA_H
#define CAMERA_H

#include "tiny_vec.h"



class camera
{
public:
	

	void set_resolution(int resx, int resy);

	//get ray origin
	virtual tiny_vec<float,3> get_origin()=0;

	//get ray direction through pixel(i,j) 
	virtual tiny_vec<float,3> get_direction(int i, int j, float di=0.5f, float dj =0.5f)=0;

	//image width in pixel
	int width();
	
	//image height in pixel
	int height();
	
	

protected:
	
	int res_u,res_v;
};

#endif

