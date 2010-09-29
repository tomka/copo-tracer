// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#include "blinn_phong.h"
#include "math_utils.h"

tiny_vec<float,3> blinn_phong::shade(intersection_info* hit, light_source *light)
{
		
	tiny_vec<float,3> attenuation = light->get_attenuation_factors(hit); 
	tiny_vec<float,3> col(0.0f,0.0f,0.0f);

	//diffuse term
	float NLc = clamp(dot(hit->get_normal(),hit->get_light_dir()),0.0f,1.0f);
	col += NLc*diffuse*attenuation*light->get_color();
	
	//specular term
	if(dot(hit->get_normal(),hit->get_light_dir()))
	{
		tiny_vec<float,3> V = -hit->get_incoming_ray().get_direction();
		tiny_vec<float,3> H = hit->get_light_dir()+V;
		H.normalize();
		float NHc = clamp(dot(hit->get_normal(),H),0.0f,1.0f);
		col += specular*std::pow(NHc,shininess)*attenuation*light->get_color();
	}

	return col;	

}

