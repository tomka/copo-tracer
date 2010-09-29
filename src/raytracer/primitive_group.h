// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#ifndef PRIMITIVE_GROUP
#define PRIMITIVE_GROUP

#include "primitive.h"
#include <vector>

class primitive_group: public primitive
{
public:
	
	virtual ~primitive_group();

	virtual bool closest_intersection(intersection_info* hit, float min_lambda);

	virtual bool any_intersection(ray<float>& r,float min_lambda, float max_lambda);

	virtual void add_primitive(primitive* p);
	
	virtual void clear();
	
	
protected:
	std::vector<primitive*> primitives;

};

#endif