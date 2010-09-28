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