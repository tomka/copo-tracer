// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#ifndef SAMPLER_2D_H
#define SAMPLER_2D_H

#include "mat.h"

template <typename T>
class sampler_2d
{
public:
	enum wrapping_type { REPEAT,  CLAMP_TO_EDGE, CLAMP_TO_BORDER, MIRROR_REPEAT};
	enum interpolation_type { BILINEAR, NEAREST};

	sampler_2d(mat<T>* img=NULL);
	void bind_image(const mat<T>* img);

	void set_wrapping(wrapping_type wrap);
	wrapping_type get_wrapping() const;

	void set_border_color(const T& color);
	T get_border_color() const;

	interpolation_type get_interpolation() const;
	void set_interpolation(interpolation_type inter);

	T operator()(int x, int y) const;
	T operator()(float x, float y) const;
	T operator()(double x, double y) const;

	int w() const;
	int h() const;

protected:
	const mat<T> *img_ptr;
	T border_color;
	wrapping_type wrapping;
	interpolation_type interpolation;

	bool is_inside(int i, int j) const;
	T sample_nearest(float x, float y) const;
	T sample_bilinear(float x, float y) const;
	
};

template <typename T>
int sampler_2d<T>::w() const
{
	return img_ptr->w();
}

template <typename T>
int sampler_2d<T>::h() const
{
	return img_ptr->h();
}

template <typename T>
sampler_2d<T>::sampler_2d(mat<T>* img):img_ptr(img)
{
	border_color=0;
	wrapping = MIRROR_REPEAT;
	interpolation = NEAREST;
}

template <typename T>
void sampler_2d<T>::bind_image(const mat<T>* img)
{
	img_ptr = img;
}

template <typename T>
void sampler_2d<T>::set_wrapping(wrapping_type wrap)
{
	wrapping = wrap;
}

template <typename T>
typename sampler_2d<T>::wrapping_type sampler_2d<T>::get_wrapping() const
{
	return wrapping;	
}

template <typename T>
typename sampler_2d<T>::interpolation_type sampler_2d<T>::get_interpolation() const
{
	return interpolation;
}

template <typename T>
void sampler_2d<T>::set_interpolation(interpolation_type inter)
{
	interpolation = inter;
}

template <typename T>
void sampler_2d<T>::set_border_color(const T& color)
{
	border_color = color;
}

template <typename T>
T sampler_2d<T>::get_border_color() const
{
	return border_color;
}

template <typename T>
bool sampler_2d<T>::is_inside(int i, int j) const
{
	if(i <0 || j < 0 || i >= w() || j >= h())
		return false;
	else
		return true;
}

template <typename T>
T sampler_2d<T>::sample_nearest(float x, float y) const
{
	int i = (int)floor(x+0.5f);
	int j = (int)floor(y+0.5f);
	return operator()(i,j);
}


template <typename T>
T sampler_2d<T>::sample_bilinear(float x, float y) const
{
	int i0 = (int)floor(x);
	int j0 = (int)floor(y);
	int i1 = i0+1;
	int j1 = j0+1;
	float factorx = clamp(x-i0,0.0f,1.0f);
	float factory = clamp(y-j0,0.0f,1.0f);
	
	return  (1.0f-factory)*((1.0f-factorx)*operator()(i0,j0) + factorx*operator()(i1,j0))
		          +factory*((1.0f-factorx)*operator()(i0,j1) + factorx*operator()(i1,j1));
}

inline int repeat(int x, int w)
{
	int v =x%w;
	if(v < 0)
		return w+v;
	else
		return v;
		
}

inline int mirror(int x, int w)
{
	int two_w = 2*w;
	x =repeat(x,two_w);

	if(x < w)
		return x;
	else
		return two_w-x-1;
}



template <typename T>	
T sampler_2d<T>::operator()(int x, int y)const
{
	switch(wrapping)
	{
		case REPEAT:
			x = repeat(x,img_ptr->w());
			y = repeat(y,img_ptr->h());
			return (*img_ptr)(x,y);			

		case CLAMP_TO_EDGE:
			x = clamp<int>(x,0,img_ptr->w()-1);
			y = clamp<int>(y,0,img_ptr->h()-1);
			return (*img_ptr)(x,y);

		case CLAMP_TO_BORDER:
			if(is_inside(x,y))
				return (*img_ptr)(x,y);
			else
				return border_color;

		case MIRROR_REPEAT:
		default:
			x = mirror(x,img_ptr->w());
			y = mirror(y,img_ptr->h());
			return (*img_ptr)(x,y);

	}
}

template <typename T>
T sampler_2d<T>::operator()(float x,float y) const
{
	switch(interpolation)
	{		
	case BILINEAR:
		return sample_bilinear(x,y);
	case NEAREST:
	default:
		return sample_nearest(x,y);
	};
}

template <typename T>
T sampler_2d<T>::operator()(double x,double y) const
{
	switch(interpolation)
	{		
	case BILINEAR:
		return sample_bilinear((float)x,(float)y);
	case NEAREST:
	default:
		return sample_nearest((float)x,(float)y);
	};
}



#endif