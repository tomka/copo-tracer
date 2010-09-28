#include "projector_light.h"




tiny_vec<float,3> projector_light::get_attenuation_factors(const intersection_info* hit)
{
	tiny_vec<float,3> pw = hit->get_location();
	tiny_vec<float,2> pi =project(pw);
	if(dot(R.row(2),pw-get_eye())<0)
		return tiny_vec<float,3>(0.0f,0.0f,0.0f);
	
	
	return light_source::get_attenuation_factors(hit)*projector_image(pi[0],pi[1]);

}

void projector_light::calc_light_direction_and_distance(intersection_info *hit)
{
	tiny_vec<float,3> light_dir = get_eye()-hit->get_location();
	hit->set_light_distance(light_dir.length());
	hit->set_light_dir( light_dir/hit->get_light_distance());
}

tiny_vec<float,2> projector_light::project(const tiny_vec<float,3>& p)
{
	tiny_vec<float,3> xh = K*(R*p+t);
	xh/=xh[2]; 
	return tiny_vec<float,2>(xh[0],xh[1]);
}

tiny_vec<float,3> projector_light::get_eye()
{
	return -transpose(R)*t;
}

void projector_light::set_look_at(const tiny_vec<float,3>& eye, const tiny_vec<float,3>& center,const tiny_vec<float,3>& up)
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


	t =-R*eye;

}

projector_light::projector_light(mat<tiny_vec<float,3> >* img,float fovy)
{
	projector_image.set_interpolation(sampler_2d<tiny_vec<float,3>>::BILINEAR);
	projector_image.set_wrapping(sampler_2d<tiny_vec<float,3>>::CLAMP_TO_BORDER);
	projector_image.set_border_color(tiny_vec<float,3>(0,0,0));

	projector_image.bind_image(img);
	float f = 1.0f/tan(fovy*3.14159f/360.0f);
	float aspect = img->w()/(float)img->h();
	K = eye<float>(3);
	K(0,2) = img->w()/2.0f;
	K(1,2) = img->h()/2.0f;
	K(0,0) = img->w()/2.0f*f/aspect;
	K(1,1) = img->h()/2.0f*f;
	R=eye<float>(3);

}


projector_light::projector_light()
{
	projector_image.set_interpolation(sampler_2d<tiny_vec<float,3>>::BILINEAR);
	projector_image.set_wrapping(sampler_2d<tiny_vec<float,3>>::CLAMP_TO_BORDER);
	projector_image.set_border_color(tiny_vec<float,3>(0,0,0));

	K = eye<float>(3);
	R = eye<float>(3);
}



void projector_light::set_projector_image(mat<tiny_vec<float,3> >* img,float fovy)
{
	projector_image.bind_image(img);
	
	float f = 1.0f/tan(fovy*3.14159f/360.0f);
	float aspect = img->w()/(float)img->h();
	K(0,2) = img->w()/2.0f;
	K(1,2) = img->h()/2.0f;
	K(0,0) = img->w()/2.0f*f/aspect;
	K(1,1) = img->h()/2.0f*f;
}



