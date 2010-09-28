// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#include "triangle.h"
#include <limits>



triangle::triangle(const tiny_vec<float,3>& p0, const tiny_vec<float,3>& p1, const tiny_vec<float,3>& p2,
	const tiny_vec<float,3>& n1, const tiny_vec<float,3>& n2, const tiny_vec<float,3>& n3)
{
	v0=p0;
	edge1=p1-p0;
	edge2=p2-p0;
	nml1=n1;
	nml2=n2;
	nml3=n3;
	
	nml1.normalize();nml2.normalize();nml3.normalize();

	normal = cross(edge1,edge2);
	normal.normalize();
}
	
triangle::triangle(const tiny_vec<float,3>& p0, const tiny_vec<float,3>& p1, const tiny_vec<float,3>& p2)
	
{
	v0.set(p0(0),p0(1),p0(2));
	edge1=p1-p0;
	edge2=p2-p0;
	
	normal = cross(edge1,edge2);
	normal.normalize();
	nml1 = nml2 = nml3 = normal;
	
}



std::pair<tiny_vec<float,3>, tiny_vec<float,3> > triangle::calc_bounds()
{
	tiny_vec<float,3> minb(v0(0),v0(1),v0(2));
	tiny_vec<float,3> maxb(v0(0),v0(1),v0(2));

	for(int i = 0; i < 3; i++)
	{
		if(minb[i] > v0[i]+edge1[i])
			minb[i] = v0[i]+edge1[i];
		if(maxb[i] < v0[i]+edge1[i])
			maxb[i] = v0[i]+edge1[i];
		if(minb[i] > v0[i]+edge2[i])
			minb[i] = v0[i]+edge2[i];
		if(maxb[i] < v0[i]+edge2[i])
			maxb[i] = v0[i]+edge2[i];
		minb-=0.001f;
		maxb+=0.001f;
	
	
	}
	return std::make_pair(minb,maxb);
}

void triangle::calc_normal(intersection_info* hit, float  u, float v)
{
	float w = 1-u-v;
	tiny_vec<float,3> nml = u*nml2+v*nml3+w*nml1;
	nml.normalize();
	/*if(dot(nml,hit->get_incoming_ray().get_direction()) > 0)
		nml*=-1.0f;*/
	hit->set_normal(nml);
	
}

bool triangle::closest_intersection(intersection_info* hit, float min_lambda)
{


	tiny_vec<float,3> tvec,pvec,qvec;
	tiny_vec<float,3> orig = hit->get_incoming_ray().get_origin();
	tiny_vec<float,3> dir = hit->get_incoming_ray().get_direction();
	


	pvec = cross(dir,edge2);
	float det = dot(edge1,pvec);	
	//no culling
	
	float eps2 =0;
	if(det > -eps2  && det < eps2)
		return false;

	float inv_det = 1.0f/det;
	tvec = orig-v0;

	float u = dot(tvec,pvec)*inv_det;
	if(u < 0.0f || u > 1.0f)
		return false;

	qvec = cross(tvec,edge1);

	float v = dot(dir,qvec)*inv_det;
	if(v < 0.0f || u+ v > 1.0f)
		return false;

	float t = dot(edge2,qvec)*inv_det;


	if(t >= min_lambda && t < hit->get_lambda())
	{
		hit->set_lambda(t);
		hit->set_object(this);
		calc_normal(hit,u,v);
		return true;
	}
	return false;
}

bool triangle::any_intersection(ray<float>& r,float min_lambda, float max_lambda)
{


	tiny_vec<float,3> tvec,pvec,qvec;
	tiny_vec<float,3> orig = r.get_origin();
	tiny_vec<float,3> dir = r.get_direction();
	
	pvec = cross(dir,edge2);

	float det = dot(edge1,pvec);
	
	float eps2 = 0.0f;
	if(det > -eps2  && det < eps2)
		return false;

	float inv_det = 1.0f/det;
	tvec = orig-v0;

	float u = dot(tvec,pvec)*inv_det;
	if(u < 0.0 || u > 1.0)
		return false;

	qvec = cross(tvec,edge1);

	float v = dot(dir,qvec)*inv_det;
	if(v < 0.0 || u+ v > 1.0)
		return false;

	float t = dot(edge2,qvec)*inv_det;

	return (t >= min_lambda && t < max_lambda);
	

}
