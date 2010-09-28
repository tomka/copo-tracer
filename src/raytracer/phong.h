#ifndef PHONG_H
#define PHONG_H

#include "material.h"

class phong : public material
{
public:
	
	virtual tiny_vec<float,3> shade(intersection_info* hit, light_source *light);
	
};

#endif