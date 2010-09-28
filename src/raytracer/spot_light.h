#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

#include "light_source.h"


class spot_light: public light_source
{
public:

	spot_light();
	
	void set_spot_direction(float dx, float dy, float dz);

	void set_direction(const tiny_vec<float,3>& dir);

	void set_position(float px, float py, float pz);

	void set_position(const tiny_vec<float,3>& pos);

	void set_spot_exponent(float exponent);

	float get_spot_exponent() const;

	void set_spot_cutoff(float co);

	virtual tiny_vec<float,3> get_attenuation_factors(const intersection_info* hit);
	
	virtual void calc_light_direction_and_distance(intersection_info *hit);
	
private:
	tiny_vec<float,3> spot_direction;
	tiny_vec<float,3> position;
	float spot_exponent;
	float spot_cos_cutoff;

	
};




#endif