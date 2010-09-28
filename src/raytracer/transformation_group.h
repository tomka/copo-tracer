// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#ifndef TRANSFORMATION_GROUP_H
#define TRANSFORMATION_GROUP_H

#include "primitive_group.h"




class transformation_group: public primitive_group
{
public:
	transformation_group();

	transformation_group(mat<float>& tr);

	void set_transformation(const mat<float>& T);

	virtual bool closest_intersection(intersection_info* hit, float min_lambda);

	virtual bool any_intersection( ray<float>& r,float min_lambda, float max_lambda);

private:
	mat<float> trafo;
	mat<float> trafo_inv;
	mat<float> trafo_inv_transpose;
};

#endif
