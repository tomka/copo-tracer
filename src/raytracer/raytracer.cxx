// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#pragma	warning (disable: 4996)
#include "raytracer.h"
#include <omp.h>
#include <iomanip>
#include "timing.h"
#include <limits>
#include "utils.h"
#include "progress.h"

void raytracer::setup(scene* s, int max_rec, sampling_type sampling)
{
	raster_image.resize(s->get_camera()->width(),s->get_camera()->height());
	depth_image.resize(s->get_camera()->width(),s->get_camera()->height());
	render_scene = s;
	max_recursion_level = max_rec;
	raytracer::sampling = sampling;
}

std::vector<tiny_vec<float,2> > raytracer::create_single_sample()
{
	std::vector<tiny_vec<float,2> > samples(1);
	samples[0]=tiny_vec<float,2>(0.5f,0.5f);
	return samples;
}

std::vector<tiny_vec<float,2> >  raytracer::create_random_samples(int n)
{
	std::vector< tiny_vec<float,2> > samples(n);
	for(int i = 0; i < n; i++)
		samples[i]=tiny_vec<float,2>(random(0.0f,1.0f),random(0.0f,1.0f));
	return samples;
}

std::vector<tiny_vec<float,2> > raytracer::create_grid_samples(int grid_res)
{
	float gs = 1.0f/grid_res;
	float gsh = gs*0.5f;
	int n = grid_res*grid_res;

	std::vector<tiny_vec<float,2> > samples(n);
	for(int i = 0; i < n; i++)
	{
		float di = gsh+(i % grid_res)*gs;
		float dj = gsh + (i / grid_res)*gs;
		samples[i]=tiny_vec<float,2>(di,dj);
	}

	return samples;
}

mat<tiny_vec<unsigned char,3> > raytracer::render()
{
	tic();
	int resx = raster_image.w();
	int resy = raster_image.h();

	camera * cam = render_scene->get_camera();
	tiny_vec<float,3> orig = cam->get_origin();

    /* Assume for now that tha camere is in space and _not_
     * within an object. A proper test should be added to
     * the primitives to test if an object contains a point.
     */
    bool origin_inside = false;

	int npixel = resx*resy;
	int ncomplete = 0;	

	#pragma omp parallel for schedule(dynamic,5)
	for(int y = 0; y < resy; ++y)
	{
		std::vector<tiny_vec<float,2> >samples;
		switch(sampling)
		{
			case NO_SUPER_SAMPLING:
				samples = create_single_sample();
				break;
			case SUPER_SAMPLING_GRID_33:
				samples = create_grid_samples(3);
				break;
			case SUPER_SAMPLING_RANDOM_4:
				samples = create_random_samples(4);
				break;
			case SUPER_SAMPLING_RANDOM_8:
				samples = create_random_samples(8);
				break;
			case SUPER_SAMPLING_RANDOM_16:
				samples = create_random_samples(16);
				break;
		}

		int ns = (int)samples.size();
		for(int x=0; x < resx; ++x)
		{
			tiny_vec<float,3> dir;
			intersection_info hit;
			tiny_vec<float,3> col(0,0,0);
			float depth = 0.0f;

			for(int s = 0; s < (int)samples.size(); s++)
			{
				dir = cam->get_direction(x,y,samples[s][0],samples[s][1]);
				hit = intersection_info(ray<float>(orig,dir),std::numeric_limits<float>::infinity());
				col += trace(&hit, origin_inside, 0);
				depth += hit.get_lambda();
			}

			col /= (float)ns;
			depth /= (float)ns;
			col.clamp(0,1);
			depth_image(x,y) = depth;
			raster_image(x,y) = float_2_uchar(col);
		}

		#pragma omp critical
		{
			ncomplete += resx;
			progress((float)ncomplete,(float)npixel);
		}
	}

	std::cout << "\n"<<std::endl;

	toc();
	return raster_image;
}

tiny_vec<float,3> raytracer::trace( intersection_info* hit, bool inside, int rec_level)
{
	if(render_scene->get_root()->closest_intersection(hit,0.001f)) //if intersection is found...
	{
		int nl = render_scene->num_lights();
		light_source *light;

		tiny_vec<float,3> color(0.0f,0.0f,0.0f);

		for(int i = 0; i < nl; ++i) // for each light...
		{
			light = render_scene->get_light(i);
			light->calc_light_direction_and_distance(hit);
			
			bool shadow=false;
			if(light->is_casting_shadows())
			{
                /* to find out if the point is in shadow we must
                 * shoot a "shadow ray" in the direction of the
                 * light. If there is any intersection on the way,
                 * the pount is in shadow, otherwise not.
                 */
				ray<float> shadow_ray(hit->get_location(),hit->get_light_dir());
				shadow = render_scene->get_root()->any_intersection(shadow_ray,0.001f,hit->get_light_distance());
			}

			if(!shadow) // not in shadow ?
				 color += hit->get_object()->get_material()->shade(hit,light);

			if(rec_level < max_recursion_level)
			{
				float refl = hit->get_object()->get_material()->get_reflectivity();
				if(refl > 0)
				{
					intersection_info hit_reflect(ray<float>(hit->get_location(),hit->get_reflected_dir()),std::numeric_limits<float>::infinity());
					hit_reflect.set_object(hit->get_object());
					color += refl*trace(&hit_reflect, inside, rec_level+1);
				}

				float refrac = hit->get_object()->get_material()->get_refractivity();
				if(refrac > 0.00001)
				{
					float n1,n2;
					if(inside)
					{
						n1 = hit->get_object()->get_material()->get_index_of_refraction();
						n2 = render_scene->get_index_of_refraction();
					}
					else
					{
						n1 = render_scene->get_index_of_refraction();
						n2 = hit->get_object()->get_material()->get_index_of_refraction();
					}
					bool total_reflection;

					tiny_vec<float,3> refract_ray = hit->get_refracted_dir(n1,n2,total_reflection);

					intersection_info hit_refract( ray<float>( hit->get_location(), refract_ray ), std::numeric_limits<float>::infinity() );
					
					if(total_reflection)
						color += refrac*trace(&hit_refract,inside,rec_level+1);
					else
						color += refrac*trace(&hit_refract,!inside,rec_level+1);
				}
			}
		}

	    return color;
	}
	else
		return render_scene->get_clear_color();
}


