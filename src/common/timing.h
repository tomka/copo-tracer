// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#ifndef TIMING_H
#define TIMING_H
#include <iostream>

#ifdef WIN32

#include <windows.h>
static LARGE_INTEGER _tstart, _tend, _freq;


inline void tic()
{
	QueryPerformanceFrequency(&_freq);
	QueryPerformanceCounter(&_tstart);
}


inline double toc(bool message=true)
{
	
	QueryPerformanceCounter(&_tend);
	double diff= double (_tend.QuadPart - _tstart.QuadPart)/(_freq.QuadPart) ;
	if(message)
		std::cout << "Elapsed time is "<<diff<<" seconds."<<std::endl;
	return diff;
}


#else

#include <sys/time.h>
static struct timeval _tstart, _tend;
static struct timezone _tz;



inline void tic()
{
    gettimeofday(&_tstart, &_tz);
}

inline double toc(bool message=true)
{

    gettimeofday(&_tend,&_tz);
    double t1, t2;

    t1 =  (double)_tstart.tv_sec + (double)_tstart.tv_usec/(1000*1000);
    t2 =  (double)_tend.tv_sec + (double)_tend.tv_usec/(1000*1000);

    double diff = t2-t1;
	if(message)
		std::cout << "Elapsed time is "<<diff<<" seconds."<<std::endl;
	return diff;


}




#endif
#endif



