#include "light_source.h"
#include "utils.h"



void light_source::set_color( tiny_vec<float,3>& col ) 
{ 
	color = col; 
}

void light_source::set_color( float r, float g, float b) 
{
	color[0]=r; 
	color[1]=g; 
	color[2]=b;
}

tiny_vec<float,3>& light_source::get_color() 
{
	return color; 
}

const tiny_vec<float,3>& light_source::get_color() const
{ 
	return color; 
}

void light_source::set_attenuation(float const_att, float linear_att, float quadratic_att)
{
	const_attenuation =const_att;
	linear_attenuation=linear_att;
	quadratic_attenuation=quadratic_att;
}

tiny_vec<float,3> light_source::get_attenuation_factors(const intersection_info* hit)
{
	float d = hit->get_light_distance();                  
	float att = 1.0f/(const_attenuation + linear_attenuation*d +quadratic_attenuation*d*d);
	return tiny_vec<float,3>(att,att,att);
}


bool light_source::is_casting_shadows()
{
	return cast_shadows;
}

void light_source::disable_shadows()
{
	cast_shadows = false;
}

void light_source::enable_shadows()
{
	cast_shadows = true;
}


light_source::light_source():color(0.0f,0.0f,0.0f),cast_shadows(true),const_attenuation(1.0f),linear_attenuation(0.0f),quadratic_attenuation(0.0f)
{
}
	
	



