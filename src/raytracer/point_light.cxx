// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#include "point_light.h"


point_light::point_light() : position(0,10,0)
{
}

point_light::point_light(float px, float py, float pz) : position(px,py,pz)
{
}
	
void point_light::set_position(float px, float py, float pz)
{
	position[0] = px;
	position[1] = py;
	position[2] = pz;
	
}
void point_light::set_position(const tiny_vec<float,3>& pos)
{
	position=pos;
}

void point_light::calc_light_direction_and_distance(intersection_info *hit)
{
	tiny_vec<float,3> light_dir = position-hit->get_location();
	hit->set_light_distance(light_dir.length());
	hit->set_light_dir( light_dir/hit->get_light_distance());
}

