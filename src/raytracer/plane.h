#ifndef PLANE_H
#define PLANE_H

#include "primitive.h"

class plane: public primitive
{
public:
	plane();
	
	plane(tiny_vec<float,3>& nml, float d);

	bool closest_intersection(intersection_info* hit, float min_lambda);
	
	bool any_intersection(ray<float>& r,float min_lambda, float max_lambda); 
	

private:
	tiny_vec<float,3> normal;

	float distance;

};

#endif