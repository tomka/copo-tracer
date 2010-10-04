// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#ifndef CG_CAMERA_H
#define CG_CAMERA_H

#include "camera.h"

class cg_camera : public camera
{
public:
	cg_camera();
	
	void set_look_at(const tiny_vec<float,3>& eye, const tiny_vec<float,3>& center,const tiny_vec<float,3>& up);

	void set_perspective(float fov, float aspect, float znear, float zfar);

	void set_frustrum(float left, float right,float top,float bottom,float znear,float zfar);

	tiny_vec<float,3> get_origin();

	//get ray direction through  pixel(i,j) (di,dj) = (0.5,0.5) corresponds tothe center of the pixel
	tiny_vec<float,3> get_direction(int i, int j, float di=0.5f, float dj=0.5f);

private:
	tiny_vec<float,3> eye; 
	tiny_vec<float,3> u_dir;//x direction of camera coordinate frame
	tiny_vec<float,3> v_dir;//y direction of camera coordinate frame
	tiny_vec<float,3> w_dir;//z direction of camera coordinate frame

	float left,right,top,bottom,znear,zfar;//view frustrum distances in camera coordinate frame
};

#endif

