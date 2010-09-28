#ifndef PHASESHIFT_H
#define PHASESHIFT_H

#include "tiny_vec.h"
#include "mat.h"
#include "image_utils.h"



inline float phase_2_pixel(float phi, int w, int N)
{
	return phi*w/(2.0f*3.14159f*N);
}

inline float pixel_2_phase(float x, int w, int N)
{
	return x/w*2.0f*3.14159f*N;
}

inline rgb_image_sequence create_column_phase_shift_patterns(int resx, int resy, int N=10)
{
	rgb_image_sequence pattern_sequence;
	rgb_image I1(resx,resy);
	rgb_image I2(resx,resy);
	rgb_image I3(resx,resy);

	const float theta = 2.0f*3.14159f/3.0f;
	
	for(int x = 0; x < resx; x++)
	{
		float phi = pixel_2_phase((float)x,resx,N);

		unsigned char i1 = (unsigned char)(255.0f/2.0f*(1.0f + cos(phi-theta))) ;
		unsigned char i2 = (unsigned char)(255.0f/2.0f*(1.0f + cos(phi))      );
		unsigned char i3 = (unsigned char)(255.0f/2.0f*(1.0f + cos(phi+theta)) );
		
		for(int y = 0; y < resy; y++)
		{
			I1(x,y) = i1;
			I2(x,y) = i2;
			I3(x,y) = i3;
		}
	}

	pattern_sequence.push_back(I1);
	pattern_sequence.push_back(I2);
	pattern_sequence.push_back(I3);

	return pattern_sequence;
	
}


inline rgb_image_sequence create_row_phase_shift_patterns(int resx, int resy, int N=10)
{
	rgb_image_sequence pattern_sequence;
	rgb_image I1(resx,resy);
	rgb_image I2(resx,resy);
	rgb_image I3(resx,resy);

	const float alpha = 2.0f*3.14159f/3.0f;
	
	for(int y = 0; y < resy; y++)
	{
		float phi = pixel_2_phase((float)y,resy,N);

		unsigned char i1 = (unsigned char)(255.0f/2.0f*(1.0f + cos(phi-alpha)));
		unsigned char i2 = (unsigned char)(255.0f/2.0f*(1.0f + cos(phi))     );
		unsigned char i3 = (unsigned char)(255.0f/2.0f*(1.0f + cos(phi+alpha)) );
		
		for(int x = 0; x < resx; x++)
		{
			I1(x,y) = i1;
			I2(x,y) = i2;
			I3(x,y) = i3;
		}
	}

	pattern_sequence.push_back(I1);
	pattern_sequence.push_back(I2);
	pattern_sequence.push_back(I3);

	return pattern_sequence;
}

inline void decode_phase(const float i1,const float i2,const float i3, float& phi,float& Idc,float& Imod)
{
	const float sqrt3 = sqrt(3.0f);
	float num = sqrt3*(i1-i3);
	float denom = (2.0f*i2 - i1 -i3);
	phi = atan2(num,denom);
	if(phi < 0) 
		phi+=2.0f*3.14159f;
	Idc = (i1+i2+i3)/3.0f;
	Imod = sqrt(  (i3-i1)*(i3-i1)/3.0f    +(2.0f*i2-i1-i3)*(2.0f*i2-i1-i3)/9.0f);
}


float_image decode_phaseshift_column(const rgb_image_sequence& pattern_sequence, int N=10 )
{
	mat<float> I1 = rgb_2_gray(uchar_2_float(pattern_sequence[0]));
	mat<float> I2 =  rgb_2_gray(uchar_2_float(pattern_sequence[1]));
	mat<float> I3 =  rgb_2_gray(uchar_2_float(pattern_sequence[2]));
	int resx = I1.w();
	int resy = I1.h();
	
	mat<float> I_phi(resx,resy);
	mat<float> I_mod(resx,resy);
	mat<float> I_dc(resx,resy);
	mat<float> I_pixel(resx,resy);


	for(int y = 0; y < resy; y++)
	{
		for(int x = 0; x < resx; x++)
		{	
			decode_phase(I1(x,y),I2(x,y),I3(x,y),I_phi(x,y),I_dc(x,y),I_mod(x,y));
			I_pixel(x,y) = 	phase_2_pixel(I_phi(x,y),resx,N);	
			

		}
	}
	
	return I_pixel;
}


float_image decode_phaseshift(const rgb_image_sequence& pattern_sequence,int w, int N=10 )
{
	mat<float> I1 = rgb_2_gray(uchar_2_float(pattern_sequence[0]));
	mat<float> I2 =  rgb_2_gray(uchar_2_float(pattern_sequence[1]));
	mat<float> I3 =  rgb_2_gray(uchar_2_float(pattern_sequence[2]));
	int resx = I1.w();
	int resy = I1.h();
	
	mat<float> I_phi(resx,resy);
	mat<float> I_mod(resx,resy);
	mat<float> I_dc(resx,resy);
	mat<float> I_pixel(resx,resy);


	for(int y = 0; y < resy; y++)
	{
		for(int x = 0; x < resx; x++)
		{	
			decode_phase(I1(x,y),I2(x,y),I3(x,y),I_phi(x,y),I_dc(x,y),I_mod(x,y));
			I_pixel(x,y) = 	phase_2_pixel(I_phi(x,y),w,N);	
			

		}
	}
	
	return I_pixel;
}


#endif
