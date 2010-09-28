// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#ifdef WIN32
#pragma	warning (disable: 4996)
#endif

#include "raytracer.h"
#include "point_light.h"
#include "directional_light.h"
#include "spot_light.h"
#include "projector_light.h"
#include "box.h"
#include "plane.h"
#include "sphere.h"
#include "cylinder.h"
#include "primitive_group.h"
#include "transformation_group.h" 
#include "kd_tree.h"
#include "triangle.h"
#include "blinn_phong.h"
#include "phong.h"
#include "image_io.h"
#include "phaseshift_pattern.h"
#include "graycode_pattern.h"
#include <common/checkerboard_pattern.h>
#include <common/on_off_pattern.h>
#include "cg_camera.h"
#include "vision_camera.h"
#include <common/file_utils.h>




scene* create_scene()
{
		scene* myscene = new scene();

		int resx = 2048;
		int resy = 2*768;
	
		vision_camera *view = new vision_camera(resx,resy,45);
		view->set_look_at(tiny_vec<float,3>(6,6,20),tiny_vec<float,3>(6,2,6),tiny_vec<float,3>(0,1,0));
		myscene->set_camera(view);
		std::cout << "Camera:"<<std::endl;
		std::cout << *view <<std::endl;
		std::cout << "0 0 0 0"<<std::endl;


		myscene->set_clear_color(tiny_vec<float,3>(0.1f,0.1f,0.2f));
		
		//root node of scene
		primitive_group *group = new primitive_group();
		myscene->set_root(group);


		//several materials
		blinn_phong *redmat = new blinn_phong();
		redmat->set_diffuse(1.0f,0.0f,0.0f);
		redmat->set_specular(1.0f,1.0f,1.0f);

		blinn_phong *greenmat = new blinn_phong();
		greenmat->set_diffuse(0.0f,1.0f,0.0f);
		greenmat->set_specular(1.0f,1.0f,1.0f);
		
		blinn_phong *bluemat = new blinn_phong();
		bluemat->set_diffuse(0.0f,0.0f,1.0f);
		bluemat->set_specular(1.0f,1.0f,1.0f);

		blinn_phong *graymat = new blinn_phong();
		graymat->set_diffuse(0.8f,0.8f,0.8f);
		graymat->set_specular(1.0f,1.0f,1.0f);



		// ground plane
		primitive* pl = new plane(  tiny_vec<float,3>(0, 1, 0) , -2.0f );
		pl->set_name( "plane" );
		pl->set_material(new blinn_phong());
		pl->get_material()->set_reflectivity( 0.3f );
		pl->get_material()->set_diffuse( 0.8f,0.8f,0.8f );
		pl->get_material()->set_specular( 0.8f,0.8f,0.8f );
		group->add_primitive(pl);

		//coordinate system axis
		cylinder* cylx = new cylinder(tiny_vec<float,3>(0,0,0),tiny_vec<float,3>(5.0f,0,0),0.2f );
		cylx->set_material(redmat);
		cylx->set_name( "cylinder X" );
		cylx->set_finite(true);
		cylx->set_caps(true);
		group->add_primitive(cylx);

		cylinder* cyly = new cylinder(tiny_vec<float,3>(0,0,0),tiny_vec<float,3>(0,5.0f,0),0.2f );
		cyly->set_name( "cylinder Y" );
		cyly->set_material(greenmat);
		cyly->set_finite(true);
		cyly->set_caps(true);
		group->add_primitive(cyly);

		cylinder* cylz = new cylinder(tiny_vec<float,3>(0,0,0),tiny_vec<float,3>(0,0,5.0f),0.2f );
		cylz->set_material(bluemat);
		cylz->set_name( "cylinder Z" );
		cylz->set_finite(true);
		cylz->set_caps(true);
		group->add_primitive(cylz);
			
	
		primitive *s1 = new sphere(  5.0, 0.0, 0.0 , 0.5f );
		s1->set_name( "x sphere" );
		s1->set_material(redmat);
	
		group->add_primitive(s1);

		primitive *s2 = new sphere(  0.0, 5.0, 0.0 , 0.5f );
		s2->set_name( "y sphere" );
		s2->set_material(greenmat );
	
		group->add_primitive(s2);
	
		primitive*s3 = new sphere(  0.0, 0.0, 5.0 , 0.5f );
		s3->set_name( "z sphere" );
		s3->set_material(bluemat);
		group->add_primitive(s3);

		primitive* s4 = new sphere(  0.0f, 0.0f, 0 , 0.5f );
		s4->set_name( "center" );
		s4->set_material(graymat );
		group->add_primitive(s4);


		//another big mirroring sphere
		primitive* sp = new sphere(  0.0, 1.0, -5.0 , 3.0f );
		sp->set_name( "big sphere" );
		sp->set_material(new blinn_phong());
		sp->get_material()->set_diffuse(  1.0f, 0.0f, 1.0f  );
		sp->get_material()->set_reflectivity(0.05f);
		group->add_primitive(sp);

		//an axis aligned box	
		primitive* boxsmall = new box(  tiny_vec<float,3>(11.0f, -2.0, -4.0) ,tiny_vec<float,3>(14.0f,2.0f,0.0f ) );
		boxsmall->set_name( "box small" );
		boxsmall->set_material(new blinn_phong());
		boxsmall->get_material()->set_diffuse(  1.0f, 1.0f, 1.0f  );
		boxsmall->get_material()->set_reflectivity(0.1f);
		group->add_primitive(boxsmall);
	
		
		//a triangle mesh in a kdtree
		kd_tree *mesh = new kd_tree();
		group->add_primitive(mesh);
		mesh->add_obj_mesh("../datasets/bunny.obj");
		mesh->build_tree_spatial_median();
		
	
		triangle *tri = new triangle(tiny_vec<float,3>(3.0f,0.0f,-10.0f),
			tiny_vec<float,3>(7.0f,0.0f,-10.0f),
			tiny_vec<float,3>(0.0f,7.0f,-10.0f));
		tri->set_material(new phong());
		tri->get_material()->set_diffuse(0.4f,0.8f,0.4f);
		group->add_primitive(tri);
		
		//a transformed axis aligned box

		transformation_group* t1 = new transformation_group();
		t1->set_transformation( translate_44<float>(12.0f,0.0f,-20.0f)*rotatex_44<float>(45.0f)*rotatez_44<float>(45.0f));
		group->add_primitive(t1);
		
	
		primitive* box2 = new box(  tiny_vec<float,3>(-1.0, -1.0, -1.0) ,tiny_vec<float,3>(1.0,1.0f,1.0f ) );
		box2->set_material(new blinn_phong());
		box2->get_material()->set_diffuse(  1.0f, 1.0f, 1.0f  );
		box2->get_material()->set_reflectivity(0.1f);
		t1->add_primitive(box2);
		
	
		//several different light sources

		//Point Light
		point_light* light0 = new point_light(  -10, 10, 10  );
		light0->set_color(  1.0f, 1.0f, 1.0f  );		
		myscene->add_light(light0);

		
	/*	// Directional light
		directional_light *light1 = new directional_light(  0, -1.0f , 0.1f  );
		light1->set_color(  1.0f, 1.0f, 1.0f  );	
		myscene->add_light(light1);
		*/
			
		return myscene;

	}

int main(int argc, char** argv) 
{
		
	raytracer tracer;
	
	scene* my_scene = create_scene();
//	tracer.setup(my_scene,0,raytracer::SUPER_SAMPLING_GRID_33);
	tracer.setup(my_scene,0,raytracer::NO_SUPER_SAMPLING);
	
	
	projector_light *proj = new projector_light;
	proj->set_look_at(tiny_vec<float,3>(12,20,20),tiny_vec<float,3>(6,2,6),tiny_vec<float,3>(0,1,0));
	proj->set_color(0.8f,0.8f,0.8f);

    
	mat<tiny_vec<float,3> >checker = create_checker_board<float>(1024,768,40, 30);
	proj->set_projector_image(&checker,20.0f);
	my_scene->add_light(proj);


	std::cout << "Projector:"<<std::endl;
	std::cout << *proj<<std::endl;
	std::cout << "0 0 0 0"<<std::endl;


	/*{
		mat<tiny_vec<unsigned char,3> > img;
		read_bmp(img,"../temp/test2.bmp");

		
		proj->set_projector_image(img,30.0f);
		mat<tiny_vec<unsigned char,3> > image1 = tracer.render();
		write_bmp(image1,"../temp/mand.bmp");
	}*/

    
	rgb_image_sequence on_off_patterns = create_on_off_sequence(1024, 768);
	for(int i = 0; i< (int)on_off_patterns.size();i++)
	{
	
		mat<tiny_vec<float,3> > pat = uchar_2_float(on_off_patterns[i]);
		proj->set_projector_image(&pat,20.0f);	

		mat<tiny_vec<unsigned char,3> > image1 = tracer.render();
		if(i == 0)
			write_bmp(image1,"../datasets/on_off_graycode/on.bmp");
		if(i == 1)
			write_bmp(image1,"../datasets/on_off_graycode/off.bmp");
	}
    

	/*mat<float> depth_image = tracer.get_depth_image();
	
	for(int y = 0; y < depth_image.h(); y++)
	{
		for(int x = 0; x < depth_image.w(); x++)
		{
			if(depth_image(x,y) == std::numeric_limits<float>::infinity())
				depth_image(x,y) = 0;		
		}
	}

	write_bmp(float_2_uchar(gray_2_rgb(depth_image)),"../datasets/on_off_graycode/depth.bmp");*/
	
    
	rgb_image_sequence gcpatterns = create_column_graycode_sequence(1024, 768);
	for(int i = 0; i< (int)gcpatterns.size();i++)
	{
	
		mat<tiny_vec<float,3> > pat = uchar_2_float(gcpatterns[i]);
		proj->set_projector_image(&pat,20.0f);	

		mat<tiny_vec<unsigned char,3> > image1 = tracer.render();
		write_bmp(image1,get_numbered_filename("../datasets/on_off_graycode/gc",i,"bmp"));
	}
    

	
	





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
