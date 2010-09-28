#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "light_source.h"

class directional_light: public light_source
{
public:
	directional_light();

	directional_light(float dx, float dy, float dz);

	directional_light(const tiny_vec<float,3>& dir);

	void set_direction(float dx, float dy, float dz);
	
	void set_direction(const tiny_vec<float,3>& dir);

	virtual tiny_vec<float,3> get_attenuation_factors(const intersection_info* hit);
	
	virtual void calc_light_direction_and_distance(intersection_info *hit);
	
	
private:
	tiny_vec<float,3> direction;
	
	
};



#endif