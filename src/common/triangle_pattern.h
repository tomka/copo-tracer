#ifndef TRIANGLE_PATTERN
#define TRIANGLE_PATTERN

#include "progress.h"
#include "math_utils.h"
#include "mat.h"
#include "image_utils.h"







rgb_image create_triangle_pattern(int resx, int resy, float N)
{
	rgb_image triangle_pattern(resx,resy);
	
	for(int y = 0; y < resy; y++)
		for(int x = 0; x < resx; x++)
			triangle_pattern(x,y) = (1-abs(1 - mod((2.0f*N*x)/resx,2.0f))) * tiny_vec<unsigned char,3>(255,255,255);
		
	return triangle_pattern;
	
}


//resx column resolution of projector image
//resy row resolution of projector
//returns a  sequence of two patterns the first is completely white the second is completely black 
inline rgb_image_sequence create_triangle_sequence(const int resx,const int resy)
{
	rgb_image_sequence pattern_sequence;
		
	
	pattern_sequence.push_back( create_triangle_pattern(resx, resy, 0.5f));
	pattern_sequence.push_back( create_triangle_pattern(resx, resy, 1.0f));
	pattern_sequence.push_back( create_triangle_pattern(resx, resy, 5.0f));
	pattern_sequence.push_back( create_triangle_pattern(resx, resy, 10.0f));
	pattern_sequence.push_back( create_triangle_pattern(resx, resy, 20.0f));
	
	return pattern_sequence;
}

#endif

