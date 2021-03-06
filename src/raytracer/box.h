// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#ifndef BOX_H
#define BOX_H

#include "primitive.h"

class box : public primitive
{

	
public: 
	
    box();
    box(tiny_vec<float,3> minimum, tiny_vec<float,3> maximum);
	virtual ~box();


	std::pair<tiny_vec<float,3>, tiny_vec<float,3> > calc_bounds();

    void extend(const tiny_vec<float,3> a);

	void extend(const std::pair<tiny_vec<float,3>, const tiny_vec<float,3> >& minmax);
 
    void extend(const box& b);
 
    void clear();

	tiny_vec<float,3>& get_min();

	tiny_vec<float,3>& get_max();
 
	tiny_vec<float,3> get_extends();

	tiny_vec<float,3> get_center();

	 
 
	
     std::pair<float, float> intersect(const ray<float> &r) const;

	void calc_normal(intersection_info* hit);

	bool closest_intersection(intersection_info* hit, float min_lambda);

	bool any_intersection(ray<float>& r,float min_lambda, float max_lambda);
private:
	tiny_vec<float,3> minimum; 
    tiny_vec<float,3> maximum; 


 };

#endif
