// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#ifndef _SCENES_DEFAULT_H_
#define _SCENES_DEFAULT_H_

#ifdef WIN32
#pragma	warning (disable: 4996)
#endif

#include <raytracer/raytracer.h>
#include <raytracer/point_light.h>
#include <raytracer/directional_light.h>
#include <raytracer/spot_light.h>
#include <raytracer/projector_light.h>
#include <raytracer/box.h>
#include <raytracer/plane.h>
#include <raytracer/sphere.h>
#include <raytracer/cylinder.h>
#include <raytracer/primitive_group.h>
#include <raytracer/transformation_group.h>
#include <raytracer/kd_tree.h>
#include <raytracer/triangle.h>
#include <raytracer/blinn_phong.h>
#include <raytracer/phong.h>
#include <raytracer/cg_camera.h>
#include <raytracer/vision_camera.h>

scene* create_scene(int resx, int resy)
{
		scene* myscene = new scene();

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

		// yet another big mirroring sphere
		primitive* sp2 = new sphere(  3.0, 1.5, -5.0 , 3.0f );
		sp2->set_name( "big sphere 2" );
		sp2->set_material(new blinn_phong());
		sp2->get_material()->set_diffuse(  1.0f, 1.0f, 0.0f  );
		sp2->get_material()->set_reflectivity(0.05f);
        sp2->get_material()->set_index_of_refraction(1.5);
		group->add_primitive(sp2);
		
        //an axis aligned box	
		primitive* boxsmall = new box(  tiny_vec<float,3>(11.0f, -2.0, -4.0) ,tiny_vec<float,3>(14.0f,2.0f,0.0f ) );
		boxsmall->set_name( "box small" );
		boxsmall->set_material(new blinn_phong());
		boxsmall->get_material()->set_diffuse(  1.0f, 1.0f, 1.0f  );
		boxsmall->get_material()->set_reflectivity(0.1f);
		group->add_primitive(boxsmall);
	
		
		//a triangle mesh in a kdtree
		kd_tree *mesh = new kd_tree();
		mesh->add_obj_mesh("../datasets/warncone.obj");
		mesh->build_tree_spatial_median();
		mesh->set_material(new blinn_phong());
	    mesh->get_material()->set_diffuse(  0.5f, 0.5f, 0.0f  );
		mesh->get_material()->set_reflectivity(0.1f);
		group->add_primitive(mesh);

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

    	// Directional light
    	directional_light *light1 = new directional_light(  0, -1.0f , 0.1f  );        light1->set_color(  1.0f, 1.0f, 1.0f  );
		myscene->add_light(light1);

		return myscene;
}

#endif

