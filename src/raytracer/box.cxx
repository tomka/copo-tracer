#include "box.h"
#include <limits>



 
	
box::box() 
{
	clear(); 
}

box::box(tiny_vec<float,3> minimum, tiny_vec<float,3> maximum) : minimum(minimum),maximum(maximum) 
{
}

box::~box()
{
}


std::pair<tiny_vec<float,3>, tiny_vec<float,3> > box::calc_bounds()
{
	return std::make_pair(minimum,maximum);
}

void box::extend(tiny_vec<float,3> a)
{ 
	for(int i = 0; i < 3;i++)
	{
		minimum(i) > a(i) ? minimum(i)=a(i):minimum(i)=minimum(i);
		maximum(i) < a(i) ? maximum(i)=a(i):maximum(i)=maximum(i);
	}
}

void box::extend(std::pair<tiny_vec<float,3>,tiny_vec<float,3> >& minmax)
{
	extend(minmax.first);
	extend(minmax.second);
}

void box::extend(box& b)
 {
      extend(b.minimum);
      extend(b.maximum);
 }

 void box::clear()
 { 
	 minimum = tiny_vec<float,3>( std::numeric_limits<float>::infinity(),std::numeric_limits<float>::infinity(),std::numeric_limits<float>::infinity() );
     maximum = tiny_vec<float,3>( -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity() );
 }


 tiny_vec<float,3>& box::get_min()
 {
	 return minimum;
 }

 tiny_vec<float,3>& box::get_max()
 {
	 return maximum;
 }

 tiny_vec<float,3> box::get_extends()
 {
	 return 0.5f*(maximum-minimum);
 }

 tiny_vec<float,3> box::get_center()
 {
	 return get_extends()+get_min();
 }

 


 std::pair<float, float> box::intersect(const ray<float> &r) const
 {   
	 float entry, exit;
	 tiny_vec<float,3> orig = r.get_origin();
	 tiny_vec<float,3> dir = r.get_direction();
	 tiny_vec<float,3> inv_dir = r.get_inv_direction();
	

    // x axis
     float xmin = (minimum[0] - orig[0]) * inv_dir[0];
     float xmax = (maximum[0] - orig[0]) * inv_dir[0];
	 if(xmin > xmax) std::swap(xmin,xmax);

     // y axis
     float ymin = (minimum[1] -orig[1]) * inv_dir[1];
     float ymax = (maximum[1] - orig[1]) * inv_dir[1];
	
	 if(ymin > ymax) std::swap(ymin,ymax);
     if(xmin > ymax || ymin > xmax) 
		 return std::make_pair(std::numeric_limits<float>::infinity(),-std::numeric_limits<float>::infinity());
	 
     entry = (std::max)(xmin,ymin);
     exit = (std::min)(xmax,ymax);

     // z axis
     float zmin = (minimum[2] - orig[2]) * inv_dir[2];
     float zmax = (maximum[2] - orig[2]) * inv_dir[2];
	 if(zmin > zmax) std::swap(zmin,zmax);
    
    if(entry > zmax || zmin > exit) 
             return std::make_pair((std::numeric_limits<float>::infinity)(),-(std::numeric_limits<float>::infinity)());

    entry = (std::max)(entry,zmin);
    exit  = (std::min)(exit,zmax);

     return std::make_pair(entry,exit);
 }

void box::calc_normal(intersection_info* hit)
{
	tiny_vec<float,3> ex1 = hit->get_location()-get_min();
	tiny_vec<float,3> ex2 = hit->get_location()-get_max();
	
	int idx = -1;
	float d = std::numeric_limits<float>::infinity();
	float s =0;
	for(int i = 0; i < 3;i++)
	{
		if(fabs(ex1[i]) < d)
		{
			idx = i;
			d = fabs(ex1[i]);
			s = -1.0f;
		}
		if(fabs(ex2[i]) < d)
		{
			idx = i;
			d = fabs(ex2[i]);
			s = 1.0f;
		}
	}
	tiny_vec<float,3> nml(0,0,0);
	nml(idx)=s;
	
	
	
	hit->set_normal(nml);
	

	

}

bool box::closest_intersection(intersection_info* hit, float min_lambda)
{

			
	std::pair<float,float> intersections = intersect(hit->get_incoming_ray());
	
	 if(intersections.first > min_lambda  &&  intersections.first < hit->get_lambda())
	 {
		hit->set_lambda(intersections.first);
		hit->set_object(this);	
		
		calc_normal(hit);
		return true;
	 }
	 if(intersections.second > min_lambda  &&  intersections.second < hit->get_lambda())
	 {
		 
		hit->set_lambda(intersections.second);
		hit->set_object(this);	
		calc_normal(hit);
		return true;
	}
	return false;
	
}

 bool box::any_intersection(ray<float>& r,float min_lambda, float max_lambda)
 {
	// if(dont_hit == this)
	//	return false;
	 std::pair<float,float> intersections = intersect(r);
	
	  
	 if(intersections.first > min_lambda  &&  intersections.first < max_lambda)
		return true;
	 
	 if(intersections.second > min_lambda  &&  intersections.second < max_lambda)
		return true;

	 return false;
 }



