// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H

#include "mat.h"
#include "tiny_mat.h"
#include "tiny_vec.h"
#include <limits>

typedef mat<tiny_vec<unsigned char,3> > rgb_image;
typedef mat<unsigned char> grayscale_image;
typedef mat<float> float_image;
typedef mat<tiny_vec<float,2> > vec2f_image;

typedef std::vector<rgb_image> rgb_image_sequence;
typedef std::vector<grayscale_image> grayscale_image_sequence;
typedef std::vector<float_image> float_image_sequence;

//compute gradient field by forward differences
inline vec2f_image grad(const float_image& u, float s=1.0f)
{
	int w = u.w();
	int h = u.h();
	vec2f_image g(w,h);
	
	for(int j = 0; j < h; j++)
	{
		for(int i = 0; i < w; i++)
		{
			if ( i == w-1)
				g(i,j)[0] = 0.0f;
			else
				g(i,j)[0] = (u(i+1,j) - u(i,j))/s;

			if ( j == h-1)
				g(i,j)[1] = 0.0f;
			else
				g(i,j)[1] = (u(i,j+1) - u(i,j))/s;
			
		}
	}
	return g;
}

//returns a structur tensor image of u given the gradient image of u nabla_u
inline mat<tiny_mat<float,2,2> > struc_tensor(mat<tiny_vec<float,2> >& nabla_u)
{
	int w = nabla_u.w();
	int h = nabla_u.h();

	mat<tiny_mat<float,2,2> > st(w,h);

	for(int j = 0; j < h; j++)
		for(int i = 0; i < w; i++)
			st(i,j) = dyad(nabla_u(i,j),nabla_u(i,j));

	return st;
}

//compute normal field from gradient field
inline mat< tiny_vec<float,2> > normal(mat<tiny_vec<float,2> >& nabla_u,float eps=0.001f, float s=1.0f)
{
	int w = nabla_u.w();
	int h = nabla_u.h();
	mat<tiny_vec<float,2> > n(w,h);
	
	for(int j = 0; j < h; j++)
	{
		for(int i = 0; i < w; i++)
		{
			float f = nabla_u(i,j).length();
			n(i,j) = nabla_u(i,j)/sqrt(f+eps*eps);
		}
	}
	return n;
}

//compute tangent field from normal field
inline mat<tiny_vec<float,2> > tangent(mat<tiny_vec<float,2> >& n,float eps=0.001f, float s=1.0f)
{
	int w = n.w();
	int h = n.h();
	mat<tiny_vec<float,2> > t(w,h);
	
	for(int j = 0; j < h; j++)
	{
		for(int i = 0; i < w; i++)
		{
			
			t(i,j)[0] = -n(i,j)[1];
			t(i,j)[1] =  n(i,j)[0];
		}
	}
	return n;
}

//compute divergence of vector field xi
inline mat<float> div(mat<tiny_vec<float,2> >& xi,float s=1.0f)
{
	int w = xi.w();
	int h = xi.h();
	mat<float> D(w,h);
	for(int j = 0; j < h; j++)
	{
		for(int i = 0; i < w; i++)
		{
			if(i == 0)
				D(i,j) = xi(i,j)[0]/s;
			else if( i < w-1)
				D(i,j) = (xi(i,j)[0] - xi(i-1,j)[0])/s;
			else
				D(i,j) = - xi(i-1,j)[0]/s;
			

			if(j == 0)
				D(i,j) += xi(i,j)[1]/s;
			else if(j < h-1)
				D(i,j) += (xi(i,j)[1] - xi(i,j-1)[1])/s;
			else
				D(i,j) += - xi(i,j-1)[1]/s;
			
		}
	}

	return D;
}



//normalize image between lo and hi
inline void normalize(mat<float>& img,float lo=0.0f, float hi=1.0f)
{
    typedef mat<float>::iterator m_it;
	float imin = std::numeric_limits<float>::max();
	float imax = std::numeric_limits<float>::min();

	for (m_it it = img.begin(); it != img.end(); it++) {
        float val = *it;
        if (val < imin)
            imin = val;
        if (val > imax)
            imax = val;
    }
	
	float s = (hi-lo)/(imax-imin);
	
	for(mat<float >::iterator it = img.begin();it != img.end(); it++)
	{
		*it = s*(*it-imin)+lo;
	}
}


template <typename T>
mat<tiny_vec<T,3> > gray_2_rgb(const mat<T>  & gray_img)
{
	mat<tiny_vec<T,3> > result(gray_img.w(),gray_img.h());	
	typename mat<tiny_vec<T,3> >::iterator rit=result.begin();
	
	for(typename mat<T>::const_iterator it = gray_img.begin();it != gray_img.end(); it++)
		*rit++ = gray_2_rgb(*it);

	return result;
}



inline mat<float> maximum(mat<float>& img1, mat<float>& img2)
{
	int w = img1.w();
	int h = img1.h();
	mat<float> m(w,h);
	for(int j = 0; j < h; j++)
		for(int i = 0; i < w; i++)
			m(i,j) = (std::max)(img1(i,j),img2(i,j));
	return m;
}


inline mat<tiny_vec<unsigned char,3> > double_2_uchar(const mat<tiny_vec<double,3> > & rgb_img)
{
	mat<tiny_vec<unsigned char,3> > result(rgb_img.w(),rgb_img.h());	
	mat<tiny_vec<unsigned char,3> >::iterator rit=result.begin();
	
	for(mat<tiny_vec<double,3> >::const_iterator it = rgb_img.begin();it != rgb_img.end(); it++)
		*rit++ = double_2_uchar(*it);

	return result;
}

//advice apply ROF denoising before using edge detector
//around 1 homogenous region around zero near edges
inline mat<float> edge_detector(mat<float>& u,float s=1.0f,float a=10.0f, float b=0.55f)
{
	int w = u.w();
	int h = u.h();
	
	mat<float> g(w,h);

	mat<tiny_vec<float,2> > nabla_i = grad(u,s);
	for(int j = 0; j < h; j++)
		for(int i = 0; i < w; i++)
			g(i,j) = exp(-a*pow(nabla_i(i,j).length(),b));
			
	return g;
}


//converts rgb image sequence to grayscale floating point sequence 
inline float_image_sequence rgb_sequence_2_float(const rgb_image_sequence& sequence)
{
	float_image_sequence seq_f;

	for(int i = 0; i < (int)sequence.size(); i++)
		seq_f.push_back( rgb_2_gray(uchar_2_float(sequence[i])));
	
	return seq_f;
}


inline void adaptive_threshold(const float_image& img, float_image& imgbin, int s,int t,float low=0.0f,float high=1.0f)
{
	int w=img.w();
	int h=img.h();
	float sum;
	int x1,y1,x2,y2;
	float_image intImg(w,h);
	imgbin.resize(w,h);
	float count;

	for(int i = 0; i < w;i++)
	{
		sum=0;
		for(int j = 0; j < h; j++)
		{
			sum +=img(i,j);
			if(i == 0)
				intImg(i,j)=sum;
			else
				intImg(i,j)=intImg(i-1,j)+sum;
		}
	}
		
	for(int i = 0; i < w;i++)
	{
		for(int j = 0; j <  h; j++)
		{
			x1 = i-s/2;
			if(x1 < 1)
				x1=1;
			x2 = i+s/2;
			if(x2 > w-1)
				x2=w-1;
			y1 = j-s/2;
			if(y1 < 1)
				y1=1;
			y2 = j+s/2;
			if(y2 > h-1)
				y2=h-1;
			count =(float) (x2-x1)*(y2-y1);
			sum = intImg(x2,y2)-intImg(x2,y1-1) - intImg(x1-1,y2) + intImg(x1-1,y1-1);
			if(img(i,j)*count <= ((sum*(100-t))/100))
				imgbin(i,j)=low;
			else
				imgbin(i,j)=high;
		}
	}
}

#endif
