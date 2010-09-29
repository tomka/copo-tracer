// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.


#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "scene.h"
#include "utils.h"




class raytracer
{
	
	
public:
	enum sampling_type
	{
		NO_SUPER_SAMPLING,
		SUPER_SAMPLING_GRID_33,
		SUPER_SAMPLING_RANDOM_4,
		SUPER_SAMPLING_RANDOM_8,
		SUPER_SAMPLING_RANDOM_16
	};

	
	void setup(scene* s, int max_rec, sampling_type sampling=SUPER_SAMPLING_GRID_33);

	mat<tiny_vec<unsigned char,3> > render();	

	mat<float> get_depth_image(){return depth_image;}
	

protected:
	tiny_vec<float,3> trace( intersection_info* hit, int rec_level);

	std::vector<tiny_vec<float,2> > create_single_sample();
	
	std::vector<tiny_vec<float,2> >  create_random_samples(int n);
	
	std::vector<tiny_vec<float,2> >  create_grid_samples(int grid_res);
	
	
	

private:

	mat<tiny_vec<unsigned char,3> > raster_image;
	mat<float> depth_image;
	scene* render_scene;
	int max_recursion_level;
	int num_samples;
	sampling_type sampling;
	

};

#endif
