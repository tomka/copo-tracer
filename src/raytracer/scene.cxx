// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#include "scene.h"

/* constructor which assigns a refraction index of 1.0 (air)
 * and darker blue as clear color
 */
scene::scene()
    : view(NULL), root(NULL), clear_color(0.4f,0.4f,0.8f),
      index_of_refraction(1.0)
{}

scene::~scene()
{
	if(root)
		delete root;

	//delete lights
	for(std::vector<light_source*>::iterator it = lights.begin();
		it != lights.end();
		++it)
	{
		delete *it;
	}
	//delete camera
	if(view)
		delete view;
}

void scene::add_light(light_source *l)
{
	lights.push_back(l);
}

light_source* scene::get_light( int i )
{
	return lights[i];
}

int scene::num_lights()
{
	return (int)lights.size();
}

tiny_vec<float,3> scene::get_clear_color()
{
	return clear_color;
}

void scene::set_clear_color(const tiny_vec<float,3>& color)
{
	clear_color = color;
}

camera* scene::get_camera()
{
	return view;
}

void scene::set_camera(camera* c)
{
	view = c;
}


primitive* scene::get_root()
{
	return root;
}

void scene::set_root(primitive* r)
{
	root = r;
}

float scene:: get_index_of_refraction()
{
    return index_of_refraction;
}

