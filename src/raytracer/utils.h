#pragma once
#include <cstdlib>




inline float random(float a, float b)
{
	return ((b-a)*((float)rand()/RAND_MAX))+a;
}