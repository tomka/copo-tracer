// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#include "material.h"
#include "utils.h"

// constructor
material::material()
    : diffuse(0.8f,0.8f,0.8f),specular(0.4f,0.4f,0.4f),
  	  reflection(0.0f),refraction(0.0f),shininess(80)
{
}

void material::set_diffuse( tiny_vec<float,3>& col )
{ 
	diffuse = col;
}
void material::set_diffuse( float r, float g, float b) 
{
	diffuse[0]=r;
	diffuse[1]=g;
	diffuse[2]=b;
}

tiny_vec<float,3>& material::get_diffuse()
{ 
	return diffuse; 
}

const tiny_vec<float,3>& material::get_diffuse() const
{ 
	return diffuse;
}

void material::set_specular( tiny_vec<float,3>& col )
{
	specular = col;
}

void material::set_specular( float r, float g, float b)
{
	specular[0]=r;
	specular[1]=g;
	specular[2]=b;
}

tiny_vec<float,3>& material::get_specular() 
{
	return specular;
}

const tiny_vec<float,3>& material::get_specular() const
{ 
	return specular;
}

void material::set_shininess(float shine)
{
	shininess = shine;
}

float material::get_shininess() const
{
	return shininess;
}

void material::set_reflectivity(float refl)
{
	reflection = refl;
}

float material::get_reflectivity() const 
{
	return reflection; 
}

void material::set_refractivity(float refrac)
{
	refraction = refrac;
}

float material::get_refractivity() const
{
	return refraction;
}

void material::set_index_of_refraction(float refrac_index)
{
    this->refrac_index = refrac_index;
}

float material::get_index_of_refraction() const
{
	return refrac_index;
}

