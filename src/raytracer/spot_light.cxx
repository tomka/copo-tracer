#include "spot_light.h"

spot_light::spot_light()
{
	set_position(10,10,10);
	set_spot_direction(-1,-1,-1);
	set_spot_exponent(80.0f);
	set_color(0.8f,0.8f,0.8f);
	set_spot_cutoff(30.0f);
	enable_shadows();
}

void spot_light::set_spot_direction(float dx, float dy, float dz)
{
	spot_direction[0] = dx;
	spot_direction[1] = dy;
	spot_direction[2] = dz;
	spot_direction.normalize();
}
void spot_light::set_direction(const tiny_vec<float,3>& dir)
{
	spot_direction=dir;
	spot_direction.normalize();
}
void spot_light::set_position(float px, float py, float pz)
{
	position[0] = px;
	position[1] = py;
	position[2] = pz;
	
}
void spot_light::set_position(const tiny_vec<float,3>& pos)
{
	position=pos;
}

void spot_light::set_spot_exponent(float exponent)
{
	spot_exponent=exponent;
}

float spot_light::get_spot_exponent() const
{
	return spot_exponent;
}

void spot_light::set_spot_cutoff(float co)
{
	spot_cos_cutoff=cos(co*3.14159f/180.0f);
}



tiny_vec<float,3> spot_light::get_attenuation_factors(const intersection_info* hit)
{
	float att=0.0f;
	float spotdot =	dot(-hit->get_light_dir(),spot_direction);
	if(spotdot >= spot_cos_cutoff)
		 att = std::pow(spotdot,spot_exponent);
	return light_source::get_attenuation_factors(hit)*tiny_vec<float,3>(att,att,att);
}

void spot_light::calc_light_direction_and_distance(intersection_info *hit)
{
	tiny_vec<float,3> light_dir = position-hit->get_location();
	hit->set_light_distance(light_dir.length());
	hit->set_light_dir( light_dir/hit->get_light_distance());
	
}


