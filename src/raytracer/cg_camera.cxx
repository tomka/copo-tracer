#include "cg_camera.h"
#include "utils.h"


void cg_camera::set_look_at(const tiny_vec<float,3>& eye, const tiny_vec<float,3>& center,const tiny_vec<float,3>& up)
{
	cg_camera::eye = eye;
	w_dir = eye-center;
	w_dir.normalize();

	v_dir = up;
	v_dir.normalize();
	
	u_dir = cross(v_dir,w_dir);
	u_dir.normalize();

	v_dir = cross(w_dir,u_dir);
	v_dir.normalize();
}

void cg_camera::set_perspective(float fov, float aspect, float znear, float zfar)
{

	top = znear*tan(fov*3.14159f/360.0f);
	bottom = -top;
	right = aspect *top;
	left = -right;
	cg_camera::zfar = zfar;
	cg_camera::znear = znear;
}

void cg_camera::set_frustrum(float left, float right,float top,float bottom,float znear,float zfar)
{
	cg_camera::left = left;
	cg_camera::right = right;
	cg_camera::top = top;
	cg_camera::bottom = bottom;
	cg_camera::znear = znear;
	cg_camera::zfar = zfar;
}


tiny_vec<float,3> cg_camera::get_origin()
{
	return eye;
}

//get ray direction through center of pixel(i,j)
tiny_vec<float,3> cg_camera::get_direction(int i, int j,float di, float dj)
{
	float us =left + (right-left)*(i+di)/res_u;
	float vs =bottom + (top-bottom)*(res_v-(j+dj))/res_v;
	float ws =-znear;		
	tiny_vec<float,3> dir =  us*u_dir + vs*v_dir + ws*w_dir;
	dir.normalize();
	
	return dir;
}




cg_camera::cg_camera():eye(0,0,0),u_dir(1,0,0),v_dir(0,1,0),w_dir(0,0,1)
{
	set_perspective(45.0f, res_u/(float)res_v, 0.01f, 100.0f);
}
