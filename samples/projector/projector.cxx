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
	tracer.setup(my_scene,2,raytracer::SUPER_SAMPLING_GRID_33);
//	tracer.setup(my_scene,4,raytracer::NO_SUPER_SAMPLING);

//	projector_light *proj = new projector_light;
//	proj->set_look_at(tiny_vec<float,3>(12,20,20),tiny_vec<float,3>(6,2,6),tiny_vec<float,3>(0,1,0));
//	proj->set_color(0.8f,0.8f,0.8f);
 
//	mat<tiny_vec<float,3> >checker = create_checker_board<float>(1024,768,40, 30);
//	proj->set_projector_image(&checker,20.0f);
//	my_scene->add_light(proj);

//	std::cout << "Projector:"<<std::endl;
//	std::cout << *proj<<std::endl;
//	std::cout << "0 0 0 0"<<std::endl;

	/*{
		mat<tiny_vec<unsigned char,3> > img;
		read_bmp(img,"../temp/test2.bmp");

		proj->set_projector_image(img,30.0f);
		mat<tiny_vec<unsigned char,3> > image1 = tracer.render();
		write_bmp(image1,"../temp/mand.bmp");
	}*/
 
//	rgb_image_sequence on_off_patterns = create_on_off_sequence(1024, 768);
//	for(int i = 0; i< (int)on_off_patterns.size();i++)
//	{
//		mat<tiny_vec<float,3> > pat = uchar_2_float(on_off_patterns[i]);
//		proj->set_projector_image(&pat,20.0f);
//		mat<tiny_vec<unsigned char,3> > image1 = tracer.render();
//		if(i == 0)
//			write_bmp(image1,"../datasets/on_off_graycode/on.bmp");
//		if(i == 1)
//			write_bmp(image1,"../datasets/on_off_graycode/off.bmp");
//	}
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

//	rgb_image_sequence gcpatterns = create_column_graycode_sequence(1024, 768);
//	for(int i = 0; i< (int)gcpatterns.size();i++)
//	{
//		mat<tiny_vec<float,3> > pat = uchar_2_float(gcpatterns[i]);
//		proj->set_projector_image(&pat,20.0f);	
//		mat<tiny_vec<unsigned char,3> > image1 = tracer.render();
//		write_bmp(image1,get_numbered_filename("../datasets/on_off_graycode/gc",i,"bmp"));
//	}
/*
	std::vector<mat<tiny_vec<unsigned char,3> > > pspatterns = create_column_phase_shift_patterns(1024, 768);
	for(int i = 0; i< (int)pspatterns.size();i++)
	{
		mat<tiny_vec<float,3>> pat = uchar_2_float(pspatterns[i]);
		proj->set_projector_image(&pat,20.0f);
		mat<tiny_vec<unsigned char,3> > image1 = tracer.render();
		std::stringstream ss;
		ss <<"../temp/ps"<<i <<".bmp";
		write_bmp(image1,ss.str());
	}
	
	for(int i = 0; i< (int)gcpatterns.size();i++)
	{
		invert_pattern(gcpatterns[i]);
		mat<tiny_vec<float,3>> pat = uchar_2_float(gcpatterns[i]);
		proj->set_projector_image(&pat,20.0f);

		mat<tiny_vec<unsigned char,3> > image1 = tracer.render();
		std::stringstream ss;
		ss <<"../temp/gcinv"<<i <<".bmp";
		write_bmp(image1,ss.str());
	}*/

	return 0;
}

