// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#include "plane.h"





plane::plane():normal(0.0f,1.0f,0.0f),distance(0){}

plane::plane(const tiny_vec<float,3>& nml, float d)
{
	normal=nml;
	normal.normalize();
	distance=d; 
}


bool plane::closest_intersection(intersection_info* hit, float min_lambda)
{

	
	float nd = dot(normal,hit->get_incoming_ray().get_direction());
	if(nd == 0)
		return false;
	
	float lambda =(distance-dot(normal,hit->get_incoming_ray().get_origin()))/nd;
	if(lambda > min_lambda && lambda < hit->get_lambda())
	{
		hit->set_lambda(lambda);
		hit->set_object(this);
		
		if(dot(normal,hit->get_incoming_ray().get_direction()) < 0)
			hit->set_normal( normal);
		else
			hit->set_normal(-normal);

		return true;
	}
	return false;
	

}

bool plane::any_intersection(ray<float>& r,float min_lambda, float max_lambda) 
{

	
	float nd = dot(normal,r.get_direction());
	if(nd == 0)
		return false;
	
	float lambda =(distance-dot(normal,r.get_origin()))/nd;
	return lambda > min_lambda && lambda < max_lambda;
}
