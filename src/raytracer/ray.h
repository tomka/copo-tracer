// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#pragma once
#include <common/tiny_vec.h>
#include <common/mat.h>

template <typename T>
class ray
{
public:
	
	
	ray(){}

	ray(const ray& r):origin(r.origin),direction(r.direction),inv_direction(r.inv_direction)
	{
	}

	ray(const tiny_vec<T,3>& orig,const tiny_vec<T,3> dir)
	{ 
		set_origin(orig);
		set_direction(dir); 
	}

	ray( T ox, T oy, T oz,T dx, T dy, T dz)
	{
		set_origin(ox,oy,oz);
		set_direction(dx,dy,dz);
	}

	void set_origin(const tiny_vec<T,3>& orig)
	{
		origin=orig;
	}

	void set_origin(T ox, T oy, T oz)
	{
		origin[0] = ox;
		origin[1] = oy;
		origin[2] = oz;
	}

	tiny_vec<T,3>& get_origin()
	{
		return origin;
	}

	const tiny_vec<T,3>& get_origin()const {return origin;}
	
	
	void set_direction(T dx, T dy,T dz)
	{
		direction[0]=dx;
		direction[1]=dy;
		direction[2]=dz;
		direction.normalize();
		inv_direction[0] = 1.0f/direction[0];
		inv_direction[1] = 1.0f/direction[1];
		inv_direction[2] = 1.0f/direction[2];
	}
	void set_direction(const tiny_vec<T,3>& dir)
	{
		direction = dir;
		direction.normalize();
		inv_direction[0] = 1.0f/direction[0];
		inv_direction[1] = 1.0f/direction[1];
		inv_direction[2] = 1.0f/direction[2];

	}
	tiny_vec<T,3>& get_direction(){	return direction;}
	const tiny_vec<T,3>& get_direction()const {return direction;}

	tiny_vec<T,3>& get_inv_direction(){	return inv_direction;}
	const tiny_vec<T,3>& get_inv_direction()const {return inv_direction;}
	
	tiny_vec<T,3> eval_at(T lambda)const{return origin+lambda*direction;}

	void transform(const mat<T>& m)
	{
		set_origin(unhomog(tiny_vec<float,4>(m*homog(origin))));
		tiny_vec<float,4> dirh(direction);
		dirh[3]=0;
		dirh=m*dirh;
		tiny_vec<float,3> dir(dirh.begin(),dirh.end()-1);
		set_direction(dir);
	}

private:
	tiny_vec<T,3> origin;
	tiny_vec<T,3> direction;
	tiny_vec<T,3> inv_direction;

};
