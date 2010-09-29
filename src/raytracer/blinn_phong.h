// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#ifndef BLINN_PHONG_H
#define BLINN_PHONG_H

#include "material.h"

class blinn_phong : public material
{
public:	

	virtual tiny_vec<float,3> shade(intersection_info* hit, light_source *light);
	
};

#endif