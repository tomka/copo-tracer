#ifndef SCENE_H
#define SCENE_H
#include "light_source.h"
#include "primitive.h"
#include "camera.h"
#include <vector>

class scene
{
public:
	scene();	
	virtual ~scene();
	
	void add_light(light_source *l);
	
	light_source* get_light( int i ) ;
	
	int num_lights();
	
	tiny_vec<float,3> get_clear_color();
	void set_clear_color(const tiny_vec<float,3>& color);
	
	
	//get viewing camera
	camera* get_camera();
	void set_camera(camera* c);
	
	//get primitives
	primitive* get_root();
	void set_root(primitive* r);

private:
	primitive* root;
	std::vector<light_source*> lights;
	camera *view;
	tiny_vec<float,3> clear_color;
	
};

#endif
