// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#include "camera.h"
#include "utils.h"

void camera::set_resolution(int resx, int resy)
{
	res_u = resx;
	res_v = resy;
}

//image width in pixel
int camera::width()
{
	return res_u;
}

//image height in pixel
int camera::height()
{
	return res_v;
}

