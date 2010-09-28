#pragma once
#include "tiny_vec.h"
#include "intersection_info.h"



class light_source
{
public:
	
	void set_color( tiny_vec<float,3>& col ); 
	void set_color( float r, float g, float b);
	
	tiny_vec<float,3>& get_color(); 
	const tiny_vec<float,3>& get_color() const;

	void set_attenuation(float const_att, float linear_att, float quadratic_att);

	virtual tiny_vec<float,3> get_attenuation_factors(const intersection_info* hit);	
	virtual void calc_light_direction_and_distance(intersection_info *hit) = 0;

	
	bool is_casting_shadows();
	void disable_shadows();
	void enable_shadows();

	light_source();
	
	

protected:

	
	tiny_vec<float,3> color;
	bool cast_shadows;

	float const_attenuation,linear_attenuation,quadratic_attenuation;
};



