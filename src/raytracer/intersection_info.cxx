// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.


#include "intersection_info.h"

#include <limits>

 
void intersection_info::set_lambda(float l)
{
	lambda = l;
}


float intersection_info::get_lambda() const 
{
	return lambda;
}

void intersection_info::set_incoming_ray(const ray<float>& r)
{
	incoming_ray = r;
}

ray<float> intersection_info::get_incoming_ray() const 
{
	return incoming_ray; 
}

primitive* intersection_info::get_object() const 
{
	return object;
}

void intersection_info::set_object(primitive* obj) 
{
	object=obj;
}

tiny_vec<float,3> intersection_info::get_light_dir() const
{
	return light_dir;
}

void intersection_info::set_light_dir(const tiny_vec<float,3>& l) 
{
	light_dir=l;
}

float intersection_info::get_light_distance()const 
{
	return light_distance;
}

void intersection_info::set_light_distance(float dist)
{
	light_distance=dist;
}

tiny_vec<float,3> intersection_info::get_normal() const
{
	return normal;
}

void intersection_info::set_normal(const tiny_vec<float,3>& nml)
{
	normal=nml;
}

tiny_vec<float,3> intersection_info::get_location() const 
{
	return incoming_ray.eval_at(lambda);
}

tiny_vec<float,3> intersection_info::get_reflected_dir() const 
{
	tiny_vec<float,3> V =  incoming_ray.get_direction();
	float cosI = dot(normal, V);
	return  V -  2.0f * cosI*normal;
}

tiny_vec<float,3> intersection_info::get_refracted_dir(float n1, float n2,bool &total_reflection) const 
{
	tiny_vec<float,3> V =  incoming_ray.get_direction();
	float n = n1 / n2;
	float cosI = dot(normal, V);
	float sinT2 = n * n * (1.0f - cosI * cosI);
	if (sinT2 > 1.0f)//total reflection
	{
		total_reflection = true;
		return V -  2.0f * cosI*normal;//return reflected ray
	}
	else
	{
		total_reflection = false;
		return n * V - (n + sqrt(1.0f - sinT2)) * normal;//return refracted ray
	}
	
}

intersection_info::intersection_info():object(NULL),lambda(std::numeric_limits<float>::infinity())
{
}

intersection_info::intersection_info(const ray<float>& r, float l) : incoming_ray(r),lambda(l),object(NULL)
{
}
