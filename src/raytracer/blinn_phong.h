#ifndef BLINN_PHONG_H
#define BLINN_PHONG_H

#include "material.h"

class blinn_phong : public material
{
public:	

	virtual tiny_vec<float,3> shade(intersection_info* hit, light_source *light);
	
};

#endif