#include "directional_light.h"
#include <limits>


directional_light::directional_light()
{
	set_direction(-1.0f,-1.0f,-1.0f);
}

directional_light::directional_light(float dx, float dy, float dz)
{
	set_direction(dx,dy,dz);
}

directional_light::directional_light(const tiny_vec<float,3>& dir)
{
	set_direction(dir);
}

void directional_light::set_direction(float dx, float dy, float dz)
{
	direction[0] = dx;
	direction[1] = dy;
	direction[2] = dz;
	direction.normalize();
}

void directional_light::set_direction(const tiny_vec<float,3>& dir)
{
	direction=dir;
	direction.normalize();
}

tiny_vec<float,3> directional_light::get_attenuation_factors(const intersection_info* hit)
{
	return tiny_vec<float,3>(1.0f,1.0f,1.0f);
}


void directional_light::calc_light_direction_and_distance(intersection_info *hit)
{
	hit->set_light_dir(-direction);
	hit->set_light_distance(std::numeric_limits<float>::infinity());	
}




