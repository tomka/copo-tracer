// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#include <random>
#include "mat.h"

template <typename T>
inline T rand_uniform_int(T low,T  high)
{
		static std::variate_generator<std::default_random_engine, std::uniform_int<T> > r(std::default_random_engine(),std::uniform_int<T>(low,high)); 
		return r();
}

template <typename T>
inline T rand_uniform_real(T low,T  high)
{
		static std::variate_generator<std::default_random_engine, std::uniform_real<T> > r(std::default_random_engine(),std::uniform_real<T>(low,high)); 
		return r();
}



template<typename T>
T rand_normal(T mean, T sigma)
{
	static std::variate_generator<std::default_random_engine, std::normal_distribution<T> > r(std::default_random_engine(),std::normal_distribution<T>(mean,sigma)); 
	return r();
}




//additive gaussian noise with zero mean and variance var
inline void gaussian_noise(mat<float>& img, float var=0.01f)
{
	float sigma = sqrt(var);
	std::for_each(img.begin(),img.end(),[&](float& elem){ elem += rand_normal<float>(0.0f,sigma);});
}

//creates salt and pepper noise on p percent of the pixel
inline void salt_pepper_noise(mat<float>& img, float p=0.05f)
{
	int N = img.size();
	int M = (int)(p*N+0.5f);
	int *indices = new int[N];
	for(int i = 0;i < N; ++i)
		indices[i] = i;

	std::random_shuffle(indices,indices+N);
	for(int i = 0; i < M; ++i)
		*(img.begin()+indices[i])=(float)rand_uniform_int<int>(0,1);
	
	delete[] indices;
}

//multiplicative gaussian noise with mean=1.0 and variance var
inline void speckle_noise(mat<float>& img, float var=0.01f)
{
	float sigma = sqrt(var);
	std::for_each(img.begin(),img.end(),[&](float& elem){ elem *= rand_normal<float>(1.0f,sigma);});
}
