#ifndef SPHERE_H
#define SPHERE_H
#include "primitive.h"


class sphere: public primitive
{
public:
	sphere();

	sphere(float cx, float cy, float cz, float r);
	
	sphere(tiny_vec<float,3>& c, float r);

	void set_radius(float r);
	
	std::pair<tiny_vec<float,3>, tiny_vec<float,3> > calc_bounds();

	bool closest_intersection(intersection_info* hit, float min_lambda);
	
	bool any_intersection(ray<float>& r,float min_lambda,float max_lambda);

protected:
	void calc_normal(intersection_info* hit) const;
	

private:
	tiny_vec<float,3> center;
	float radius;
	float sqr_radius;

};

#endif