#include "phong.h"
#include "math_utils.h"
	
tiny_vec<float,3> phong::shade(intersection_info* hit, light_source *light)
{
	tiny_vec<float,3> col(0.0f,0.0f,0.0f);
	tiny_vec<float,3> attenuation = light->get_attenuation_factors(hit); 
	
	
	//diffuse term
	float NLc = clamp(dot(hit->get_normal(),hit->get_light_dir()),0.0f,1.0f);
	col += NLc*diffuse*attenuation*light->get_color();
	
	//specular term
	if(dot(hit->get_normal(),hit->get_light_dir()))
	{
		tiny_vec<float,3> R = hit->get_reflected_dir();
		float NRc = clamp(dot(hit->get_normal(),R),0.0f,1.0f);
		col += specular*std::pow(NRc,shininess)*attenuation*light->get_color();
	}

	return col;	


}
