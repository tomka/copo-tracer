// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#pragma once
#include "../common/tiny_vec.h"
#include "intersection_info.h"
#include "light_source.h"

class material
{
public:
	material();
	
	void set_diffuse( tiny_vec<float,3>& col ) ;
	void set_diffuse( float r, float g, float b);
	tiny_vec<float,3>& get_diffuse();
	const tiny_vec<float,3>& get_diffuse() const;

	void set_specular( tiny_vec<float,3>& col ) ;
	void set_specular( float r, float g, float b);
	tiny_vec<float,3>& get_specular() ;
	const tiny_vec<float,3>& get_specular() const;

	void set_shininess(float shine);
	float get_shininess() const;

	void set_reflectivity(float refl);
	float get_reflectivity()const ;

//	void set_refractivity(float refrac) ;
//	float get_refractivity()const ;
//	float get_index_of_refraction() const;

	virtual tiny_vec<float,3> shade(intersection_info* hit,light_source *light) =0;

protected:
	
	tiny_vec<float,3> diffuse;
	tiny_vec<float,3> specular;
	float shininess;

	float reflection;
	float refraction;

	//float refrac_index;
};







