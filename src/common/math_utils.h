// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#ifndef MATH_UTILS_H
#define MATH_UTILS_H

//returns the square of x = x*x
template <typename T>
T sqr(T x)
{
	return x*x;
}

//returns the sign of x -1 if x < 0, 1 otherwise
template <typename T>
int sign(T x)
{
	if( x < 0)
		return -1;
	else
		return  1;
}

template <typename T>
T clamp(T x, T lo, T hi)
{
	if (x < lo)
		return lo;
	if(x > hi)
		return hi;
	return x;
}

//minimum of three values
template <typename T>
T (min)(T x, T y, T z)
{
	if(x < y)
		return x < z ? x : z;
	else
		return y < z ? y : z;
}

//minimum of three values
template <typename T>
T (max)(T x, T y, T z)
{
	if(x > y)
		return x > z ? x : z;
	else
		return y > z ? y : z;
}


inline int binary_2_gray(int x)
{
	return x^(x >>1);
}

inline int gray_2_binary(int n)
{
	
		for(int i= (n >> 1) ; i != 0 ; n ^= i, i >>= 1);
		return n;
}

inline bool get_ith_bit_of_x(int x, int i)
{
	return (x&(1<<i)) != 0;
}

inline void set_ith_bit_of_x(int &x,int i, bool b)
{
	if(b)
		x |= (1 << i); //set ith bit to 1
	else
		x &= ~(1 << i); //set ith bit to 0
}

inline void toggle_ith_bit_of_x(int &x,int i)
{	
	x ^= (1 << i);
}



inline float ld(float x)
{
	return log(x)/log(2.0f);
}



inline unsigned hamming_dist(unsigned x, unsigned y)
{
  unsigned dist = 0, val = x ^ y;
 
  // Count the number of set bits
  while(val)
  {
    ++dist; 
    val &= val - 1;
  }
 
  return dist;
}


//find the real roots of ax^2 +bx+c = 0
// the number of real roots is returned
template <typename T>
int solve_real_quadratic(const T& a, const T& b, const T& c, T x[2]) 
{

	if(a == 0)
	{
		if(b == 0)
			return 0;
		else
		{
			x[0] = -c/b;
			return 1;
		}
	}
	T d = b*b -  4*a*c;

	if(d >= 0)
	{
		T q = sqrt(d)/(2 *a);
		if(q == 0)
		{
			x[0] = (-b/(2 * a)) ;
			return 1;
		}
		else
		{
			x[0] = (-b/(2 * a)) + q;
			x[1] = (-b/(2 * a)) - q;
			return 2;
		}
	}
	return 0;
        
} 


//solve quadratic equation in normal form: x^2 + px + q = 0
// returns the number of real solutions
// the solutions are returned in x1 and x2 
template <typename T>
int solve_quadratic_eqn(const T& p,const T& q, T x[2])
{
	T p_half = p/2.0f;
	T D = p_half*p_half - q;
	
	if(D < 0)
		return 0;
	
	x[0] = x[1] = -p_half;
	if(D == 0)
		return 1;

	//D > 0
	D = sqrt(D);
	x[0] += D;
	x[1] -= D;
	return 2;
}
//floating point modulo
inline float mod(float x,float m)
{
	assert(m >= 0);
	if(x >=0)
		return x - floor(x/m)*m;
	else
		return x - ceil(x/m)*m;
}

#endif