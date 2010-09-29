// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#ifndef VISION_CAMERA_H
#define VISION_CAMERA_H
#include "camera.h"
#include "mat.h"

class vision_camera : public camera
{
public:
	//get ray origin
	virtual tiny_vec<float,3> get_origin();

	//get ray direction through pixel(i,j) 
	virtual tiny_vec<float,3> get_direction(int i, int j, float di=0.5f, float dj =0.5f);

	//set internal camera matrix
	void set_K(const mat<float> &K);

	//set external rotation matrix
	void set_R(const mat<float> &R);

	//set external translation vector
	void set_t(const tiny_vec<float,3> & t);

	vision_camera();

	vision_camera(int resx, int resy,float fovy);

	void set_look_at(const tiny_vec<float,3>& eye, const tiny_vec<float,3>& center,const tiny_vec<float,3>& up);

	
private:
	
	mat<float> K;
	mat<float> R;
	mat<float> Minv;
	tiny_vec<float,3> t;

	friend std::ostream& operator<<(std::ostream& out,const vision_camera& cam);
};

inline std::ostream& operator<<(std::ostream& out,const vision_camera& cam)
{
	out << cam.res_u<<std::endl;
	out << cam.res_v<<std::endl;
	out <<cam.K;
	out <<cam.R;
	out << cam.t;

	return out;
}

#endif