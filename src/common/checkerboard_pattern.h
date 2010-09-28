// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#ifndef CHECKERBOARD_H
#define CHECKERBOARD_H
#include "mat.h"
#include "image_utils.h"

//create a checker board pattern
template<typename T>
inline mat< tiny_vec<T,3> > create_checker_board(int resx, int resy,int n, int m)
{

	mat< tiny_vec<T,3> > pattern(resx,resy);

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
