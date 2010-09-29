// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#ifndef IMAGE_INPAINTING_H
#define IMAGE_INPAINTING_H

#include "image_denoising.h"


inline float cooling_constant(float theta0, float theta1,float t1)
{
	return -(log(theta1)-log(theta0))/(log(exp(1.0f))*t1);
}

inline float theta_k(float k,float theta0,float c=1)
{

	return theta0*exp(-k*c);
}


// f input image 
// X data indicator
// u inpainted image




inline void inpainting_c2f(mat<float>& f,mat<float>& u, float lambda=0.1,int K=3,float minification=0.5,bool debug=false,float theta0=1,float theta_K=0.0001)
{
	std::vector<mat<float> > pyramid_f = create_pyramid(f,minification);
	u=f;
	std::vector<mat<float> > pyramid_u = create_pyramid(f,minification);
	mat<tiny_vec<float,2>> xi(u.w(),u.h());
	xi=tiny_vec<float,2>(0.0f,0.0f);
	std::vector<mat<tiny_vec<float,2>> > pyramid_xi = create_pyramid(xi,minification);

	int L = pyramid_f.size();
	float theta;
	float c = cooling_constant(theta0,theta_K,K-1.0f);
	
	for(int l = L-1; l > 0; l--)
	{
		float s = 1.0f/pow(minification,l);
		std::cout << l<<" "<<L <<std::endl;
		ROF_denoising_PG(pyramid_f[l],pyramid_u[l],pyramid_xi[l], lambda,false,K,(float)s);
		pyramid_u[l-1] =pyramid_u[l];
		pyramid_u[l-1].resample_bilinear(pyramid_f[l-1].w(),pyramid_f[l-1].h());
		pyramid_xi[l-1] =pyramid_xi[l];
		pyramid_xi[l-1].resample_bilinear(pyramid_f[l-1].w(),pyramid_f[l-1].h());
	}
	//std::cout << "E: "<<ROF_energy_TV(pyramid_f[0],pyramid_u[0],lambda)<<std::endl;
	
	ROF_denoising_PG(pyramid_f[0],pyramid_u[0],pyramid_xi[0], lambda,false,K,1.0f,true);
	
	//std::cout << "E: "<<ROF_energy_TV(pyramid_f[0],pyramid_u[0],lambda)<<std::endl;
	//if(debug)
	//	write_bmp(pyramid_u, "../temp/debug/ROF_C2F_");
	u = pyramid_u[0];
}







inline void inpainting(const mat<float>& f, const mat<float>& X, mat<float>& u, float lambda=1.0f,int K=20,bool debug=false,float theta0=1,float theta_K=0.0001)
{
	int w = f.w();
	int h = f.h();
	u=f;
	mat<float> v = f;
	
	
	float theta;
	float c = cooling_constant(theta0,theta_K,K-1.0f);
	for(int k=0; k < K; k++)
	{
		theta = theta_k((float)k,theta0,c);
		std::cout <<"fix v minimize u"<<std::endl;
		ROF_denoising_PG_C2F(v,u,theta,6);
		
		std::cout << "fix u minimize v"<<std::endl;
	
		for(int j = 0; j < h; j++)
		{
			for(int i = 0; i < w; i++)
			{
				float a = u(i,j) - f(i,j);
				float b = X(i,j)*theta/lambda; 
				if( a > b)
					v(i,j)=u(i,j) - b;
				else if(a <-b)
					v(i,j)=u(i,j) + b;
				else
					v(i,j)=f(i,j);
	
			}
		}
		if(debug)
		{
			if(k%5)
			write_bmp(gray_2_rgb(float_2_uchar(u)),	get_numbered_filename("../temp/debug/u",k,"bmp"));
		}
		//std::cout << theta << std::endl;
	}
	
}


#endif