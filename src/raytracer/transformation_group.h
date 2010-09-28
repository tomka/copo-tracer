#ifndef TRANSFORMATION_GROUP_H
#define TRANSFORMATION_GROUP_H

#include "primitive_group.h"




class transformation_group: public primitive_group
{
public:
	transformation_group();

	transformation_group(mat<float>& tr);

	void set_transformation(mat<float>& T);

	virtual bool closest_intersection(intersection_info* hit, float min_lambda);

	virtual bool any_intersection( ray<float>& r,float min_lambda, float max_lambda);

private:
	mat<float> trafo;
	mat<float> trafo_inv;
	mat<float> trafo_inv_transpose;
};

#endif