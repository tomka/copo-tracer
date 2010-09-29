// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "light_source.h"

class point_light: public light_source
{
public:
	point_light();
	point_light(float px, float py, float pz);
	void set_position(float px, float py, float pz);
	void set_position(const tiny_vec<float,3>& pos);
	
	virtual void calc_light_direction_and_distance(intersection_info *hit);
	
	
private:
	tiny_vec<float,3> position;
};


#endif