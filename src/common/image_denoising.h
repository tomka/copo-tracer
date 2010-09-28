#ifndef IMAGE_DENOISING_H
#define IMAGE_DENOISING_H

#include "mat.h"
#include "image_utils.h"
#include "file_utils.h"




float ROF_energy_SG(mat<float>& f,mat<float>& u,float lambda)
{
	float E = 0;
	int w = f.w();
	int h= f.h();
	mat<tiny_vec<float,2>> nabla_u = grad(u);
	for(int j = 0; j < h; j++)
	{
		for(int i = 0; i < w; i++)
		{
			E+=sqr(u(i,j)-f(i,j))/(2.0f*lambda) + nabla_u(i,j).sqr_length();
		}
	}
	return E;
}
// \frac{1}{2\lambda} \int_{\Omega}(u-f)^2 + |grad(u)|_2 dx
float ROF_energy_TV(mat<float>& f,mat<float>& u,float lambda)
{
	float E = 0;
	int w = f.w();
	int h= f.h();
	mat<tiny_vec<float,2>> nabla_u = grad(u);
	for(int j = 0; j < h; j++)
	{
		for(int i = 0; i < w; i++)
		{
			E+=sqr(u(i,j)-f(i,j))/(2.0f*lambda) + nabla_u(i,j).length();
		}
	}
	return E;
}




inline mat<tiny_vec<float,2> > project_unit_ball(const mat<tiny_vec<float,2>>& xi)
{
	int w = xi.w();
	int h = xi.h();
	mat<tiny_vec<float,2> > xip(w,h);
	for(int j = 0; j < h; j++)
	{
		for(int i = 0; i < w; i++)
		{
			xip(i,j) = xi(i,j)/(std::max)(1.0f,xi(i,j).length());
		}
	}
	return xip;	
}

//ROF denoising with projected gradient method
inline void ROF_denoising_PG(mat<float>& f,mat<float>& u,mat<tiny_vec<float,2>>& xi, float lambda=0.1f,bool not_initialised=true,int K=20, float s = 1.0f,bool debug=false)
{
	float theta = 1.0f/(4.0f*lambda);
	int w = f.w();
	int h = f.h();
	

	if(not_initialised)
	{
		u = f;
		xi.resize(w,h);
		xi = tiny_vec<float,2>(0.0f,0.0f);
	}
	for(int k = 0; k < K; k++)
	{
		u = f - lambda*div(xi,s);
		xi = project_unit_ball(xi - theta*grad(u,s));	
		//if(debug)
			//std::cout << "E: "<<ROF_energy_TV(f,u,lambda)<<std::endl;
	}
}





//todo nu upsampling!!!!!!
//f input image
//u initial guess if u_not_initialised == false /output image
//K number of iterations s grid size (for image pyramid use
inline void ROF_denoising_FPG(mat<float>& f,mat<float>& u,mat<tiny_vec<float,2>>& nu, float lambda=0.1,bool not_initialised=true,int K=20,float s=1.0f)
{
	float theta = 1.0f/(8.0f*lambda);
	float tk = 0.0f;
	float tk_plus_1;
	
	int w = f.w();
	int h = f.h();

	mat<tiny_vec<float,2> > xi_k(w,h);
	mat<tiny_vec<float,2> > xi_k_plus_1(w,h);

	
	if(not_initialised)
	{
		nu.resize(w,h);
		nu = xi_k = tiny_vec<float,2>(0.0f,0.0f);
		u = f;
	}
	for(int k = 0; k < K; k++)
	{
	
		
		xi_k_plus_1 =  project_unit_ball(nu - theta*grad(u,s));

		tk_plus_1 = 0.5f*(1 + sqrt(1.0f+4.0f*tk*tk));
		nu = xi_k_plus_1 + (tk-1)/(tk_plus_1)*(xi_k_plus_1 -xi_k);
		xi_k = xi_k_plus_1;
		tk = tk+1;
		u = f - lambda*div(nu);
	}	
}


template <typename T>
std::vector<mat<T> > create_pyramid(mat<T> img,float minification=0.8f, int minres=20, int maxlevel=10)
{
	std::vector<mat<T> > pyramid;
	pyramid.push_back(img);
	while((int)pyramid.back().w() > minres || (int)pyramid.back().h() > minres)
	{
		pyramid.push_back(pyramid.back());
		pyramid.back().resample_bilinear((int)(pyramid.back().w()*minification),(int)(pyramid.back().h()*minification));
	}
	return pyramid;
}

void write_bmp(std::vector<mat<float> >& pyramid, const std::string& filename_without_ending)
{
	int L = pyramid.size();
	//std::cout << "levels = "<<L<<std::endl;
	for(int l = 0; l < L; l++)
	{
		write_bmp(gray_2_rgb(float_2_uchar(pyramid[l])),get_numbered_filename(filename_without_ending,l,"bmp",2));
	}
}



inline void ROF_denoising_FPG_C2F(mat<float>& f,mat<float>& u, float lambda=0.1,int K=3,float minification=0.8,bool debug=false)
{
	std::vector<mat<float> > pyramid_f = create_pyramid(f,minification);
	u=f;
	std::vector<mat<float> > pyramid_u = create_pyramid(f,minification);
	mat<tiny_vec<float,2>> nu(u.w(),u.h());
	nu=tiny_vec<float,2>(0.0f,0.0f);
	std::vector<mat<tiny_vec<float,2>> > pyramid_nu = create_pyramid(nu,minification);

	int L = pyramid_f.size();
	
	for(int l = L-1; l > 0; l--)
	{
		float s = 1.0f/pow(minification,l);
		ROF_denoising_FPG(pyramid_f[l],pyramid_u[l],pyramid_nu[l], lambda,false,K,(float)s);
		pyramid_u[l-1] =pyramid_u[l];
		pyramid_u[l-1].resample_bilinear(pyramid_f[l-1].w(),pyramid_f[l-1].h());
		pyramid_nu[l-1] =pyramid_nu[l];
		pyramid_nu[l-1].resample_bilinear(pyramid_f[l-1].w(),pyramid_f[l-1].h());
	}
	//std::cout << "E: "<<ROF_energy_TV(f,pyramid_u[0],lambda)<<std::endl;
	ROF_denoising_FPG(pyramid_f[0],pyramid_u[0],pyramid_nu[0], lambda,false,K,1.0f);

	//if(debug)
	//	write_bmp(pyramid_u, "../temp/debug/ROF_C2F_");
	u = pyramid_u[0];
}




inline void ROF_denoising_PG_C2F(mat<float>& f,mat<float>& u, float lambda=0.1,int K=3,float minification=0.5,bool debug=false)
{
	std::vector<mat<float> > pyramid_f = create_pyramid(f,minification);
	u=f;
	std::vector<mat<float> > pyramid_u = create_pyramid(f,minification);
	mat<tiny_vec<float,2>> xi(u.w(),u.h());
	xi=tiny_vec<float,2>(0.0f,0.0f);
	std::vector<mat<tiny_vec<float,2>> > pyramid_xi = create_pyramid(xi,minification);

	int L = pyramid_f.size();
	
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


inline mat<float> ROF_denoising_PG(mat<float>& f, float lambda=0.1,int K=20,float minification=0.8f)
{
	mat<float> u;
	
	ROF_denoising_PG_C2F( f, u,lambda,K,minification,false);
	return u;
}


#endif