#pragma	warning (disable: 4996)
#include "vision_camera.h"



void vision_camera::set_look_at(const tiny_vec<float,3>& eye, const tiny_vec<float,3>& center,const tiny_vec<float,3>& up)
{
	
	tiny_vec<float,3> w_dir = -(eye-center);
	w_dir.normalize();

	tiny_vec<float,3> v_dir = up;
	v_dir.normalize();
	
	tiny_vec<float,3> u_dir = -cross(v_dir,w_dir);
	u_dir.normalize();

	v_dir = cross(w_dir,u_dir);
	v_dir.normalize();
	R.set_row(0,u_dir);
	R.set_row(1,v_dir);
	R.set_row(2,w_dir);
	Minv=inv_33(K*R);

	t =-R*eye;

}


tiny_vec<float,3> vision_camera::get_origin()
{
	
	return -transpose(R)*t;
}

void vision_camera::set_K(const mat<float> &K)
{
	vision_camera::K=K;
	Minv= inv_33(K*R);
}
void vision_camera::set_R(const mat<float> &R)
{
	vision_camera::R=R;
	Minv= inv_33(K*R);
}

void vision_camera::set_t(const tiny_vec<float,3> & t)
{
	vision_camera::t=t;
}

//get ray direction through center of pixel(i,j)
tiny_vec<float,3> vision_camera::get_direction(int i, int j, float di, float dj)
{
	

	float px = i+di;
	float py = j+dj;
	tiny_vec<float,3> x(px,py,1);
	tiny_vec<float,3> dir = Minv*x;
	dir.normalize();
	return dir;
}

vision_camera::vision_camera()
{
	t.set(0,0,0);
	R=eye<float>(3);
	K=eye<float>(3);
	Minv=inv_33(K*R);
}



vision_camera::vision_camera(int resx, int resy,float fovy)
{
	set_resolution(resx,resy);
	
	float f = 1.0f/tan(fovy*3.14159f/360.0f);
	float aspect = resx/(float)resy;
	K = eye<float>(3);
	K(0,2) = resx/2.0f;
	K(1,2) = resy/2.0f;
	K(0,0) = resx/2.0f*f/aspect;
	K(1,1) = resy/2.0f*f;
	R=eye<float>(3);
	t.zeros();
	Minv=inv_33(K*R);
	

}