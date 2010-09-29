// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#include "primitive_group.h"



primitive_group::~primitive_group()
{
	clear();
}

bool primitive_group::closest_intersection(intersection_info* hit, float min_lambda)
{
	

	bool intersect=false;
	for(std::vector<primitive*>::iterator it = primitives.begin();
		it != primitives.end();
		++it) //find closest intersection 
	{
		
		if( (*it)->closest_intersection(hit,min_lambda))
			intersect = true;
	}
	return intersect;
}

bool primitive_group::any_intersection(ray<float>& r,float min_lambda, float max_lambda)
{
	
	
			
	for(std::vector<primitive*>::iterator it = primitives.begin();
		it != primitives.end();
		++it) 
	{
	
		if( (*it)->any_intersection(r,min_lambda,max_lambda))
			return true;
	}
	return false;

}

void primitive_group::add_primitive(primitive* p)
{
	primitives.push_back(p);
}



void primitive_group::clear()
{
	for(std::vector<primitive*>::iterator it = primitives.begin();
		it != primitives.end();
		++it) 
	{
		delete *it;
	}

	primitives.clear();
}

	