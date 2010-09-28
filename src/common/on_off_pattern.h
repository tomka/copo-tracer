#ifndef ON_OFF_PATTERN
#define ON_OFF_PATTERN
#include "image_utils.h"


//resx column resolution of projector image
//resy row resolution of projector
//returns a  sequence of two patterns the first is completely white the second is completely black 
inline rgb_image_sequence create_on_off_sequence(int resx, int resy)
{
	rgb_image_sequence pattern_sequence;
	
	rgb_image on(resx,resy);
	on.fill(tiny_vec<unsigned char,3>(255,255,255));
	pattern_sequence.push_back(on);

	rgb_image off(resx,resy);
	off.fill(tiny_vec<unsigned char,3>(0,0,0));
	pattern_sequence.push_back(off);
	return pattern_sequence;

}

//computes image of ambient and active illumination 
 inline void process_on_off_sequence(const rgb_image_sequence& on_off, float_image& I_ambient, float_image & I_white)
 {
	I_ambient = rgb_2_gray(uchar_2_float(on_off[1]));
	I_white = rgb_2_gray(uchar_2_float(on_off[0])) - I_ambient;
 }

#endif