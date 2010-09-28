#include "cylinder.h"
#include "math_utils.h"

cylinder::cylinder():bottom(0,0,0),top(0,1,0),radius(0.25),finite(true),caps(true)
{
}
	
cylinder::cylinder(tiny_vec<float,3>& p1, tiny_vec<float,3>& p2,const float& r)
{
	bottom = p1;
	top = p2;
	radius = r;
	finite=true;
	caps=true;
}

void cylinder::calc_normal(intersection_info* hit)
{
	tiny_vec<float,3> l = hit->get_location();
	tiny_vec<float,3> p = l-top;
	tiny_vec<float,3> q = l-bottom;
	tiny_vec<float,3> Va = (top-bottom);
	Va.normalize();
	tiny_vec<float,3> nml;

	
	bool capnml = false;
	if(caps)
	{
		
		if(p.sqr_length() < radius*radius && std::abs(dot(p,Va)) < 0.001)
		{
			nml =  Va;
			capnml=true;
		}
		if(q.sqr_length() < radius*radius && std::abs(dot(q,Va)) < 0.001)
		{
			nml = -Va;
			capnml=true;
		}


	}
	if(!capnml)
	{
	
		 nml = l-dot(q,Va)*Va;
		nml.normalize();

	}
		
	if(dot(nml,hit->get_incoming_ray().get_direction()) < 0)
		hit->set_normal(nml);
	else
		hit->set_normal (-nml);
	

	
}

bool cylinder::closest_intersection(intersection_info* hit, float min_lambda)
{
//	if(dont_hit == this)
	//	return false;
	tiny_vec<float,3> V = hit->get_incoming_ray().get_direction();
	tiny_vec<float,3> P = hit->get_incoming_ray().get_origin();
	tiny_vec<float,3> Va = top-bottom;
	Va.normalize();
	tiny_vec<float,3> dp = P - bottom;
	tiny_vec<float,3> Avec =V-dot(V,Va)*Va;
	tiny_vec<float,3> Cvec = dp - dot(dp,Va)*Va;
	float A = Avec.sqr_length();
	float B = 2*dot(Avec,Cvec);
	float C = Cvec.sqr_length() - radius*radius;
	float ts[4];
	int n=0;

	if(caps)
	{
		ts[n]=dot(Va,bottom-P)/dot(Va,V);
		tiny_vec<float,3> Q = P + ts[n] * V;
		if( (Q-bottom).sqr_length() < radius*radius)
			n++;

		ts[n]=dot(Va,top-P)/dot(Va,V);
		Q = P + ts[n] * V;
		if( (Q-top).sqr_length() < radius*radius)
			n++;
	}
		
	int nroots= solve_real_quadratic(A, B, C, &(ts[n]));
	
	if(finite)
	{
		if(nroots > 0)
		{
		
			tiny_vec<float,3> Q = P + ts[n] * V;
			if ( dot(Q-bottom,Va) > 0 && dot(Q-top,Va) < 0)
				n++;
			else
				ts[n] =ts[n+1];
			if(nroots > 1)
			{
				tiny_vec<float,3> Q = P + ts[n] * V;
				if ( dot(Q-bottom,Va) > 0 && dot(Q-top,Va) < 0)
					n++;
			}
		}
		
	}
	

	bool ret = false;
	for(int i = 0; i < n;i++)
	{
		
		if(ts[i] > min_lambda && ts[i] < hit->get_lambda())
		{
			hit->set_lambda(  ts[i]);
			hit->set_object ( this);
			calc_normal(hit);
			ret = true;
		}
	}
	
	

	return ret;
}

bool cylinder::any_intersection(ray<float>& r,float min_lambda, float max_lambda) 
{
//	if(dont_hit == this)
	//	return false;
	tiny_vec<float,3> V = r.get_direction();
	tiny_vec<float,3> P = r.get_origin();
	tiny_vec<float,3> Va = top-bottom;
	Va.normalize();
	tiny_vec<float,3> dp = P - bottom;
	tiny_vec<float,3> Avec =V-dot(V,Va)*Va;
	tiny_vec<float,3> Cvec = dp - dot(dp,Va)*Va;
	float A = Avec.sqr_length();
	float B = 2*dot(Avec,Cvec);
	float C = Cvec.sqr_length() - radius*radius;
	float ts[4];
	int n=0;

	if(caps)
	{
		ts[n]=dot(Va,bottom-P)/dot(Va,V);
		tiny_vec<float,3> Q = P + ts[n] * V;
		if( (Q-bottom).sqr_length() < radius*radius)
			n++;

		ts[n]=dot(Va,top-P)/dot(Va,V);
		Q = P + ts[n] * V;
		if( (Q-top).sqr_length() < radius*radius)
			n++;
	}
		
	int nroots= solve_real_quadratic(A, B, C, &(ts[n]));
	
	if(finite)
	{
		if(nroots > 0)
		{
		
			tiny_vec<float,3> Q = P + ts[n] * V;
			if ( dot(Q-bottom,Va) > 0 && dot(Q-top,Va) < 0)
				n++;
			else
				ts[n] =ts[n+1];
			if(nroots > 1)
			{
				tiny_vec<float,3> Q = P + ts[n] * V;
				if ( dot(Q-bottom,Va) > 0 && dot(Q-top,Va) < 0)
					n++;
			}
		}
		
	}
	

	for(int i = 0; i < n;i++)
	{
		
		if(ts[i] > min_lambda && ts[i] < max_lambda)
			return true;
		
	}
	

	return false;
}

void cylinder::set_finite(bool b)
{
	finite = b;
}

bool cylinder::is_finite()
{
	return finite;
}

void cylinder::set_caps(bool b)
{
	caps = b;
	if(caps)
		finite=true;
}

bool cylinder::has_caps()
{
	return caps;
}

