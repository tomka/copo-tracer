// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#ifdef WIN32
#pragma	warning (disable: 4996)
#endif

#include <raytracer/raytracer.h>
#include <raytracer/projector_light.h>
#include <common/image_io.h>
#include <common/phaseshift_pattern.h>
#include <common/graycode_pattern.h>
#include <common/checkerboard_pattern.h>
#include <common/on_off_pattern.h>
#include <raytracer/cg_camera.h>
#include <raytracer/vision_camera.h>
#include <common/file_utils.h>
#include "../scenes/default.h"

int main(int argc, char** argv)
{
	raytracer tracer;
	
	scene* my_scene = create_scene( 2048, 2*768 );
//	tracer.setup(my_scene, 2, raytracer::SUPER_SAMPLING_RANDOM_8);
	tracer.setup(my_scene, 1, raytracer::SUPER_SAMPLING_GRID_33);
//	tracer.setup(my_scene, 2, raytracer::NO_SUPER_SAMPLING);

	mat<tiny_vec<unsigned char,3> > image1 = tracer.render();
	write_bmp(image1,"../datasets/results/scene.bmp");

	mat<float> depth_image = tracer.get_depth_image();
	for(int y = 0; y < depth_image.h(); y++)
	{
		for(int x = 0; x < depth_image.w(); x++)
		{
			if(depth_image(x,y) == std::numeric_limits<float>::infinity())
				depth_image(x,y) = 0;
		}
	}
	write_bmp(float_2_uchar(gray_2_rgb(depth_image)),"../datasets/results/depth.bmp");

    return 0;
}

