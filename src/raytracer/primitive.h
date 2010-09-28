#ifndef MATERIAL_H
#define MATERIAL_H

#include "intersection_info.h"
#include "material.h"


class primitive
{

public:
	void set_name(const std::string& s);

	std::string get_name() const;

	primitive();
	
	primitive(material* m);
	

	void set_material(material* m);
	
	material* get_material() const;
	

	virtual std::pair<tiny_vec<float,3>, tiny_vec<float,3> > calc_bounds();

	virtual bool closest_intersection(intersection_info* hit,float min_lambda)  =0;
	
	virtual bool any_intersection( ray<float>& r,float min_lambda, float max_lambda) =0;


protected:
	std::string name;
	material* pmaterial;
	
};

#endif




