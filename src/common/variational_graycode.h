// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#include <limits>
#include "mat.h"
#include "image_utils.h"
#include "progress.h"
#include "graycode_pattern.h"




//compute discrete gradient by forward differences using neumann boundary conditions
inline vec2f_image compute_gradient(const float_image& u)
{
	int w = u.w();
	int h = u.h();
	vec2f_image g(w,h);
	
	#pragma omp parallel for 
	for(int j = 0; j < h; j++)
	{
		for(int i = 0; i < w; i++)
		{
			if ( i == w-1)
				g(i,j)[0] = 0.0f;
			else
				g(i,j)[0] = (u(i+1,j) - u(i,j));

			if ( j == h-1)
				g(i,j)[1] = 0.0f;
			else
				g(i,j)[1] = (u(i,j+1) - u(i,j));			
		}
	}
	return g;
}


//computes divergence of 2d vector field xi 
//it is discretized with backward differences using Dirichlet boundary conditions
inline float_image compute_divergence(vec2f_image& xi)
{
	int w = xi.w();
	int h = xi.h();
	float_image D(w,h);

	#pragma omp parallel for 
	for(int j = 0; j < h; j++)
	{
		for(int i = 0; i < w; i++)
		{
			if(i == 0)
				D(i,j) = xi(i,j)[0];
			else if( i < w-1)
				D(i,j) = (xi(i,j)[0] - xi(i-1,j)[0]);
			else
				D(i,j) = - xi(i-1,j)[0];
			
			if(j == 0)
				D(i,j) += xi(i,j)[1];
			else if(j < h-1)
				D(i,j) += (xi(i,j)[1] - xi(i,j-1)[1]);
			else
				D(i,j) += - xi(i,j-1)[1];			
		}
	}

	return D;

}



float I_i(float I_ambient, float I_white,int i,float u)
{
	int uq = (int)floor(u);
	int gc = uq^(uq >>1);
	float gi;
	
	if( gc & (1<<i) )
		gi = 1.0f;
	else
		gi = 0.0f;

	return I_ambient + gi*I_white;
}


float sum(const float_image& img)
{
	float s=0.0f;
	int n =(int)img.size();

	#pragma omp parallel  reduction(+: s)        
	for(int i = 0; i < n; i++)
	{           
		s = s + img(i);  
	}
	return s;
}


// computes the energy of the reconstructed correspondence map u 
// I_ambient, I_white are input reference images
// e_data is an image containing the per pixel energy contributions of the data term
// e_smooth is an image containing the per pixel energy contributions of the smoothness term
// the integrated energy value of e_data and e_smooth is returned
float compute_original_energy(const float_image& I_ambient, const float_image& I_white,
	const float_image_sequence I_measured,
	const float lambda, const float_image& u,
	float_image& e_data,
	float_image& e_smooth)
{
	int w = I_ambient.w();
	int	h = I_ambient.h();
	int n = (int)I_ambient.size();
	int m = I_measured.size();

	vec2f_image u_grad = compute_gradient(u);
	e_data.resize(w,h);
	e_smooth.resize(w,h);
	
	#pragma omp parallel for 
	for(int i = 0; i < n; i++)
	{
		e_data(i) = 0.0f;
		for(int j = 0; j < m; j++)
		{
			float diff = abs(I_i(I_ambient(i),I_white(i),j,u(i))-I_measured[j](i));
			e_data(i) += diff;
		}

		e_smooth(i) = lambda*u_grad(i).length();

	}
	

	return sum(e_data)/m+sum(e_smooth);
}


inline float E1_pointwise(float I_ambient, float I_white, const std::vector<float>& I_m, float theta, float u, float v)
{
	float E=0.0f;
	int m = (int)I_m.size();
	
	for(int i = 0; i < m; i++)
		E += fabs(I_i(I_ambient,I_white,i,u)-I_m[i]);

	E += (u-v)*(u-v)/(2.0f*theta);

	return E/m;
}


float compute_E1_energy(const float_image& I_ambient, const float_image& I_white,
	const float_image_sequence I_measured,
	const float_image& u, const float_image& v,
	float theta)
{
	int w = I_ambient.w();
	int h= I_ambient.h();
	int m = (int)I_measured.size();
	int n = (int)I_ambient.size();
	std::vector<float> I_m(m);
	float_image e1(w,h);

	#pragma omp parallel for 
	for(int j = 0; j < n; j++)
	{
	
		for (int i = 0; i < m; i++)
			I_m[i] = I_measured[i](j);
		//compute old emin for initial u
		e1(j) = E1_pointwise(I_ambient(j), I_white(j), I_m, theta, u(j),v(j));
	}

	return sum(e1);
}





inline void minimize_E1(const int wp,const float_image& I_ambient, const float_image& I_white, const float_image_sequence I_measured, 
	float theta,const float_image& v, float_image& u)
{
	std::cout << "fix v optimize u...\n";

	int w = I_ambient.w();
	int h = I_ambient.h();
	int m = (int)I_measured.size();
	int n = (int)I_ambient.size();
	
	if(u.w() != w || u.h() != h)
		u.resize(w,h);

	std::vector<float> I_m(m);

	#pragma omp parallel for 
	for(int j = 0; j < n; j++)
	{
	
		for (int i = 0; i < m; i++)
			I_m[i] = I_measured[i](j);
		//compute old emin for initial u
		float emin = E1_pointwise(I_ambient(j), I_white(j), I_m, theta, u(j),v(j));
		//compute emin for u = v
		float ecand = E1_pointwise(I_ambient(j), I_white(j), I_m, theta, v(j),v(j));
		if(ecand < emin) 
		{
			emin = ecand;
			u(j) = v(j);
		}
		//compute emin for u= 0...1024
		for(int ui = 0; ui < wp; ui++)
		{
			ecand = E1_pointwise(I_ambient(j), I_white(j), I_m, theta, (float)ui,v(j));
			if(ecand < emin) 
			{
				emin = ecand;
				u(j) = (float)ui;
			}
		}
		
	}
	std::cout << "E1 per pixel = "<<compute_E1_energy(I_ambient,I_white,I_measured,u,v,theta)/n<<std::endl;
	
}




inline vec2f_image project(vec2f_image& p, float lambda=1.0f)
{
	int w = p.w();
	int h = p.h();
	int n = (int)p.size();
	vec2f_image pr = p;
	
	#pragma omp parallel for
	for(int i = 0; i < n; i++)
	{
		pr(i) /= std::max(1.0f,pr(i).length()/lambda);
	}
	
	return pr;
}



float compute_E2_energy(const float_image& u, const float_image& v,
	float theta, float lambda)
{
	int w = u.w();
	int h= u.h();

	int n = (int)u.size();
	
	float_image e2(w,h);
	vec2f_image vgrad = compute_gradient(v);

	#pragma omp parallel for 
	for(int j = 0; j < n; j++)
	{
	
		e2(j) = sqr(u(j)-v(j))/(2.0f*theta)+lambda*vgrad(j).length();
	}

	return sum(e2);
}



inline void minimize_E2(float lambda, float theta, float_image& v,const float_image& u, int niters=60)
{
	assert(theta > 0);
	assert(lambda > 0);

	std::cout << "fix u optimize v..."<<std::endl;
	//step size
	float tau = 1.0f/4.0f;
	int n = (int)u.size();

	int w = u.w();
	int h = u.h();
	assert(w > 0);
	assert(h > 0);

	//image of dual variable
	vec2f_image p(w,h);
	
	//initialize
	v = u;
	p = tiny_vec<float,2>(0.0f,0.0f);
	
	//iterate
	float last_energy = std::numeric_limits<float>::infinity();
	for(int k = 0; k < niters; k++)
	{
		v = u + theta* compute_divergence(p);
		p = project(p+(tau/theta)*compute_gradient(v),lambda);
		float e = compute_E2_energy(u,v,theta,lambda)/n;
		std::cout << "E2 per pixel = "<<compute_E2_energy(u,v,theta,lambda)/n<<std::endl;
		if(e >= last_energy)
			break;
		last_energy=e;
		

		
	}
	
}


inline float_image minimize_energy(int w_projector,const float_image& I_ambient, const float_image& I_white, const rgb_image_sequence& rgb_graycode_sequence,
	float lambda, int niters=10)
{
	float_image_sequence I_measured = rgb_sequence_2_float(rgb_graycode_sequence);
	
	float_image u,v;
	v = reconstruct_from_graycode(I_ambient, I_white, rgb_graycode_sequence);
	float_image edata,esmooth;
	float E = compute_original_energy(I_ambient,I_white,I_measured,lambda,v,edata, esmooth);
	std::cout << "initial energy per pixel: "<< E/I_ambient.size() <<std::endl;

	float theta=1.0f;
	for(int k = 0; k < niters; k++)
	{
		std::cout <<" ==============================="<<std::endl;
		std::cout <<"Iteration: "<<k+1<<" of "<<niters<<std::endl;
		std::cout <<"theta: "<<theta<<std::endl;
		std::cout <<"lambda: " << lambda<<std::endl;

		minimize_E1(w_projector, I_ambient, I_white, I_measured, theta,v, u);
		minimize_E2(lambda, theta,  v, u);
		theta *= 0.5f;
		
		float E = compute_original_energy(I_ambient,I_white,I_measured,lambda,v,edata, esmooth);
		std::cout << "energy per pixel: "<< E/I_ambient.size() <<std::endl;
	}
	return v;
}