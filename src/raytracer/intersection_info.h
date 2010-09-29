// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#ifndef INTERSECTION_INFO_H
#define INTERSECTION_INFO_H

#include "tiny_vec.h"
#include "ray.h"

#ifndef PRIMITIVE_H
class primitive;
#endif

class intersection_info
{
	ray<float> incoming_ray; //incident ray
	float lambda;
	primitive* object;//intersected object
	tiny_vec<float,3> normal;//surface normal at intersection point
	tiny_vec<float,3> light_dir;
	float light_distance;
public:
	void set_lambda(float l);
	float get_lambda() const;

	void set_incoming_ray(const ray<float>& r);
	ray<float> get_incoming_ray() const;
	
	primitive* get_object() const;
	void set_object(primitive* obj);

	tiny_vec<float,3> get_light_dir() const;
	void set_light_dir(const tiny_vec<float,3>& l) ;

	float get_light_distance()const;
	void set_light_distance(float dist);
	
	tiny_vec<float,3> get_normal() const;
	void set_normal(const tiny_vec<float,3>& nml);
	
	tiny_vec<float,3> get_location() const;
	tiny_vec<float,3> get_reflected_dir() const;
	tiny_vec<float,3> get_refracted_dir(float n1, float n2,bool &total_reflection) const;

	intersection_info();
	intersection_info(ray<float>& r, float l) ;
};

#endif
