// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "primitive.h"

class triangle: public primitive
{
public:

	triangle(const tiny_vec<float,3>& p0, const tiny_vec<float,3>& p1, const tiny_vec<float,3>& p2,
		const tiny_vec<float,3>& n1, const tiny_vec<float,3>& n2, const tiny_vec<float,3>& n3);
	
	triangle(const tiny_vec<float,3>& p0, const tiny_vec<float,3>& p1, const tiny_vec<float,3>& p2);

	std::pair<tiny_vec<float,3>, tiny_vec<float,3> > calc_bounds();
	
	bool closest_intersection(intersection_info* hit, float min_lambda);

	bool any_intersection(ray<float>& r,float min_lambda, float max_lambda);

protected:
	void calc_normal(intersection_info* hit, float  u, float v);
	

private:
	tiny_vec<float,3> v0,edge1,edge2;
	tiny_vec<float,3> normal;
	tiny_vec<float,3> nml1,nml2,nml3;

	
};

#endif
