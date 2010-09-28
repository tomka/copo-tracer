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

