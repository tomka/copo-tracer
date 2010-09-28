#ifndef PROJECTOR_LIGHT_H
#define PROJECTOR_LIGHT_H

#include "light_source.h"
#include "sampler_2d.h"
#include <iostream>


class projector_light: public light_source
{
public:
	projector_light(mat<tiny_vec<float,3> >* img,float fovy);

	projector_light();

	tiny_vec<float,3> get_attenuation_factors(const intersection_info* hit);

	virtual void calc_light_direction_and_distance(intersection_info *hit);
	
	void set_projector_image(mat<tiny_vec<float,3> >* img,float fovy);
	
	void set_look_at(const tiny_vec<float,3>& eye, const tiny_vec<float,3>& center,const tiny_vec<float,3>& up);




protected:
	tiny_vec<float,2> project(const tiny_vec<float,3>& p);

	tiny_vec<float,3> get_eye();



	
	
	
private:
	sampler_2d<tiny_vec<float,3> > projector_image;
	mat<float> K;
	mat<float> R;
	tiny_vec<float,3> t;

	friend  std::ostream& operator<<(std::ostream& out, const projector_light& proj);
	
	
};

inline std::ostream& operator<<(std::ostream& out, const projector_light& proj)
{
	out << proj.projector_image.w()<<std::endl;
	out << proj.projector_image.h()<<std::endl;
	out << proj.K;
	out << proj.R;
	out << proj.t;
	return out;
}

#endif