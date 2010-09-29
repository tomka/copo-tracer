// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#pragma once
#include <cstdlib>




inline float random(float a, float b)
{
	return ((b-a)*((float)rand()/RAND_MAX))+a;
}