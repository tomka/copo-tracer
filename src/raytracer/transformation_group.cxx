// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#include "transformation_group.h"




transformation_group::transformation_group()
{
	trafo = eye<float>(4);	
}

transformation_group::transformation_group(mat<float>& tr)
{
	set_transformation(tr);
}

void transformation_group::set_transformation(const mat<float>& T)
{
	trafo = T;

	trafo_inv = inv_44<float>(T);
	
	trafo_inv_transpose = transpose(trafo_inv);	
	
}

bool transformation_group::closest_intersection(intersection_info* hit, float min_lambda)
{
	//transform incoming ray  and lambda into local frame

	ray<float> temp = hit->get_incoming_ray();
	ray<float> rt=temp;
	rt.transform(trafo_inv);
	hit->set_incoming_ray(rt);
	
	bool intersect =  primitive_group::closest_intersection(hit,min_lambda);

	if(intersect)
	{
		tiny_vec<float,3> nml =unhomog(trafo_inv_transpose*homog(hit->get_normal()));
		nml.normalize();
		hit->set_normal(nml);
		
	}
	hit->set_incoming_ray(temp);
	return intersect;
	

}

bool transformation_group::any_intersection( ray<float>& r,float min_lambda, float max_lambda) 
{
	ray<float> rt = r;
	rt.transform(trafo_inv);

	return primitive_group::any_intersection(rt,min_lambda,max_lambda);

	
}

