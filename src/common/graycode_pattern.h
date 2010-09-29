// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#ifndef GRAYCODE_PATTERN
#define GRAYCODE_PATTERN

#include "progress.h"
#include "math_utils.h"
#include "mat.h"
#include "image_utils.h"




//resx column resolution of projector image
//resy row resolution of projector
//returns a graycode sequence of rgb images encoding the columns of the projector image 
//the size of the sequence depends on resx
inline rgb_image_sequence create_column_graycode_sequence(int resx, int resy)
{
	rgb_image_sequence pattern_sequence;

	int n = (int)ceil(ld((float)resx));

	for(int b = 0; b < n; b++)
	{
		rgb_image pattern(resx,resy);
		for(int x = 0; x < resx; x++)
		{
			bool bit = get_ith_bit_of_x(binary_2_gray(x),b);
			for(int y = 0; y < resy; y++)
			{
				if(bit)
					pattern(x,y).set(255,255,255);
				else
					pattern(x,y).set(0,0,0);
			}
		}
		pattern_sequence.push_back(pattern);
	}
	return pattern_sequence;
}


//resx column resolution of projector image
//resy row resolution of projector
//returns a graycode sequence of rgb images encoding the rows of the projector image 
//the size of the sequence depends on resy 
inline rgb_image_sequence create_row_graycode_sequence(int resx, int resy)
{
	rgb_image_sequence pattern_sequence;

	int n = (int)ceil(ld((float)resy));
	for(int b = 0; b < n; b++)
	{
		rgb_image pattern(resx,resy);
		for(int y = 0; y < resy; y++)
		{
			bool bit = get_ith_bit_of_x(binary_2_gray(y),b);
			for(int x = 0; x < resx; x++)
			{
				if(bit)
					pattern(x,y).set(255,255,255);
				else
					pattern(x,y).set(0,0,0);
			}
		}
		pattern_sequence.push_back(pattern);
	}
	return pattern_sequence;
}



 //compute correspondence map by decoding a graycode sequence using I_ambient and I_white 
inline float_image reconstruct_from_graycode(const float_image& I_ambient, const float_image& I_white, const rgb_image_sequence& graycode_sequence)
{
	std::cout << "start decoding graycode...\n";
	
	int n  = graycode_sequence.size();
	int resx = I_ambient.w();
	int	resy = I_ambient.h();
	float_image I_mean = I_ambient + 0.5f*I_white;
	float_image u(resx,resy);
	
	for(int y = 0;progress((float)y,(float)resy),y < resy; y++)
	{
		for(int x = 0; x < resx; x++)
		{
			
			//read graycode
			int gccode = 0;
			for(int b = 0; b < n; b++)
				set_ith_bit_of_x(gccode,b,rgb_2_gray(uchar_2_float(graycode_sequence[b](x,y))) > I_mean(x,y));
			//decode
			u(x,y) = (float)gray_2_binary(gccode);
		}
	}
	std::cout << "decoding graycode complete\n";
	return u;
}


#endif


