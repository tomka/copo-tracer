#ifndef CHECKERBOARD_H
#define CHECKERBOARD_H
#include "mat.h"
#include "image_utils.h"

//create a checker board pattern
inline rgb_image create_checkerboard_pattern(int resx, int resy,int n, int m)
{

	rgb_image pattern(resx,resy);

	for(int y = 0; y < (int)pattern.h(); y++)
	{
		for(int x = 0; x < (int)pattern.w(); x++)
		{
			if(((x/m)%2 == 0)^((y/n)%2 == 0))
				pattern(x,y).set(255,255,255);
			else
				pattern(x,y).set(0,0,0);
		}
	}
	return pattern;
}

#endif