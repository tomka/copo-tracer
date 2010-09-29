// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#include "sphere.h"



sphere::sphere():radius(1.0f),sqr_radius(1.0f)
{
	center.zeros();
}

sphere::sphere(float cx, float cy, float cz, float r):center(cx,cy,cz),radius(r)
{
	sqr_radius=r*r;
}

sphere::sphere(tiny_vec<float,3>& c, float r):center(c),radius(r)
{
	sqr_radius=r*r;
}

void sphere::set_radius(float r)
{
	radius = r;
	sqr_radius = r*r;
}

void sphere::calc_normal(intersection_info* hit)const 
{
	tiny_vec<float,3> nml = hit->get_location()-center;
	nml.normalize();
	
	hit->set_normal(nml);
}

std::pair<tiny_vec<float,3>, tiny_vec<float,3> > sphere::calc_bounds()
{
	return std::make_pair(center-radius-0.001f,center+radius+0.001f);
}

bool sphere::closest_intersection(intersection_info* hit, float min_lambda)
{
//	if(dont_hit == this)
	//	return false;

	tiny_vec<float,3> dst =  hit->get_incoming_ray().get_origin()-center;
	

	float B = dot(dst, hit->get_incoming_ray().get_direction());
	float C = dot(dst, dst) - sqr_radius;

	float D = B*B - C;
	if( D > 0)
	{ 
		float lambda = -B - sqrt(D);
		if(lambda > min_lambda && lambda < hit->get_lambda())
		{
			hit->set_lambda (lambda);
			hit->set_object (this);
			calc_normal(hit);
			return true;
		}
	}
	return false;
}

bool sphere::any_intersection(ray<float>& r,float min_lambda,float max_lambda)
{
//	if(dont_hit == this)
	//	return false;
	
	tiny_vec<float,3> oc =center-r.get_origin();
	float l2oc = dot(oc,oc);
	if (l2oc < sqr_radius) 
	{
		// starts inside of the sphere
		float tca = dot(oc, r.get_direction());			// omit division if ray.d is normalized
		float l2hc = (sqr_radius - l2oc)  + tca*tca;  // division
		float lambda =tca + sqrt(l2hc);
		
		return (lambda > min_lambda && lambda < max_lambda);	
	}
	else 
	{
		float tca = dot(oc, r.get_direction());
		if (tca < 0) // points away from the sphere
			return false;
		float l2hc = (sqr_radius - l2oc) + (tca*tca);	// division
		if(l2hc < 0)
			return false;
		float lambda =tca -sqrt(l2hc);
		return( lambda > min_lambda && lambda < max_lambda);
		
	}

}
