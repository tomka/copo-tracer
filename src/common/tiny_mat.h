// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#ifndef TINY_MAT_H
#define TINY_MAT_H

#include "tiny_vec.h"

//fixed size column major matrix
template <typename T,int M, int N >
class tiny_mat
{
	T _data[N][M];

public:
	T& operator()(int i, int j)
	{
		return _data[j][i];
	}

	T operator()(const int i,const int j) const
	{
		return _data[j][i];
	}

	int w() const
	{
		return N;
	}

	int h() const
	{
		return M;
	}

	int nrows() const
	{
		return M;
	}

	int ncols() const
	{
		return N;
	}

	tiny_mat<T,M,N>& operator*=(int s)
	{
		for(int j = 0; j < N; j++)
			for(int i = 0; i < M; i++)
				operator()(i,j)*=s;

		return *this;
	}

	tiny_mat<T,M,N> operator*(int s) const
	{
		tiny_mat<T,M,N> r =*this;
		r *= s;
		return r;
	}

	tiny_mat<T,M,N>& operator*=(float s)
	{
		for(int j = 0; j < N; j++)
			for(int i = 0; i < M; i++)
				operator()(i,j)*=s;

		return *this;
	}

	tiny_mat<T,M,N> operator*(float s) const
	{
		tiny_mat<T,M,N> r =*this;
		r *= s;
		return r;
	}

	tiny_mat<T,M,N>& operator*=(double s)
	{
		for(int j = 0; j < N; j++)
			for(int i = 0; i < M; i++) {
                operator()(i,j)*=s;
            }

		return *this;
	}

	tiny_mat<T,M,N> operator*(double s) const
	{
		tiny_mat<T,M,N> r =*this;
		r *= s;
		return r;
	}


	tiny_vec<T,M> operator*(const tiny_vec<T,N>& v) const
	{
		tiny_vec<T,M> r;
		r=0.0f;

		for(int j = 0; j < N; j++)
			for(int i = 0; i < M; i++)
				r(i) += v(j)*operator()(i,j);
		return r;
	}

	template <int P>
	tiny_mat<T,M,P> operator*(const tiny_mat<T,N,P>& m) const
	{
		
		tiny_mat<T,M,P> r;
		for(int k = 0; k < P; k++)
			for(int j = 0; j < N; j++)
				for(int i = 0; i < M; i++)
					r(i,k) += m(j,k)*operator()(i,j);
		return r;
	}

	
};


template <typename T, int M, int N>
tiny_mat<T,N,M> transpose(tiny_mat<T,M,N>& m)
{
	tiny_mat<T,N,M> mt;
	for(int j = 0; j < N; j++)
			for(int i = 0; i < M; i++)
				mt(j,i) = m(i,j);
	return mt;
}



///determinant of 2x2 matrix
template <typename T>
T det(const tiny_mat<T,2,2>& m) 
{	
	return m(0,0)*m(1,1) - m(0,1)*m(1,0);
}

///determinant of 3x3 matrix
template <typename T>
T det(const tiny_mat<T,3,3>& m) 
{	
	return m(0,0)*m(1,1)*m(2,2)
		  +m(0,1)*m(1,2)*m(2,0)
		  +m(0,2)*m(1,0)*m(2,1)
		  -m(0,0)*m(1,2)*m(2,1)
		  -m(0,1)*m(1,0)*m(2,2)
		  -m(0,2)*m(1,1)*m(2,0);
}



///compute inverse of 2x2 matrix
template <typename T>
tiny_mat<T,2,2> inv(const tiny_mat<T,2,2>& m) 
{
	
	tiny_mat<T,2,2> im;
	T t4 = 1.0 / (-m(0,0) * m(1,1) + m(0,1) * m(1,0));
	im(0,0) = -m(1,1) * t4;
	im(1,0) = m(1,0) * t4;
	im(0,1) = m(0,1) * t4;
	im(1,1) = -m(0,0) * t4;
	
	return im;
}

///compute inverse of 3x3 matrix
template <typename T>
tiny_mat<T,3,3> inv(const tiny_mat<T,3,3>& m) 
{
	
	tiny_mat<T,3,3> im;
	T t4 = m(2,0) * m(0,1);
	T t6 = m(2,0) * m(0,2);
	T t8 = m(1,0) * m(0,1);
	T t10 = m(1,0) * m(0,2);
	T t12 = m(0,0) * m(1,1);
	T t14 = m(0,0) * m(1,2);
	T t17 = (T)1.0 / (t4 * m(1,2) - t6 * m(1,1) - t8 * m(2,2) + t10 * m(2,1) + t12 * m(2,2) - t14 * m(2,1));
	im(0,0) = (m(1,1) * m(2,2) - m(1,2) * m(2,1)) * t17;
	im(0,1) = -(m(0,1) * m(2,2) - m(0,2) * m(2,1)) * t17;
	im(0,2) = (m(0,1) * m(1,2) - m(0,2) * m(1,1)) * t17;
	im(1,0) = -(-m(2,0) * m(1,2) + m(1,0) * m(2,2)) * t17;
	im(1,1) = (-t6 + m(0,0) * m(2,2)) * t17;
	im(1,2) = -(-t10 + t14) * t17;
	im(2,0) = (-m(2,0) * m(1,1) + m(1,0) * m(2,1)) * t17;
	im(2,1) = -(-t4 + m(0,0) * m(2,1)) * t17;
	im(2,2) = (-t8 + t12) * t17;
	return im;
}

//compute inverse of 4x4 matrix
template <typename T>
tiny_mat<T,4,4> inv(const tiny_mat<T,4,4>& m) 
{
	
	tiny_mat<T,4,4> im;
	T t1 = m(3,3) * m(1,1);
	T t3 = m(3,2) * m(1,1);
	T t7 = m(3,1) * m(1,2);
	T t9 = m(3,1) * m(1,3);
	T t11 = m(3,2) * m(2,1);
	T t14 = m(0,0) * m(1,1);
	T t19 = m(0,0) * m(3,3);
	T t20 = m(1,2) * m(2,1);
	T t22 = m(3,1) * m(0,0);
	T t23 = m(1,2) * m(2,3);
	T t25 = m(1,3) * m(2,2);
	T t27 = m(3,2) * m(0,0);
	T t28 = m(2,1) * m(1,3);
	T t30 = m(1,1) * m(3,0);
	T t31 = m(0,3) * m(2,2);
	T t33 = m(2,0) * m(0,3);
	T t35 = m(0,2) * m(2,3);
	T t37 = m(2,0) * m(0,2);
	T t39 = m(3,0) * m(0,2);
	T t41 = m(3,1) * m(1,0);
	T t43 = t14 * m(3,3) * m(2,2) - t14 * m(3,2) * m(2,3) - t19 * t20 + 
		t22 * t23 - t22 * t25 + t27 * t28 - t30 * t31 + t3 * t33 + t30 * t35 
		- t1 * t37 - t39 * t28 - t41 * t35;
	T t45 = m(3,0) * m(0,1);
	T t47 = m(1,0) * m(3,3);
	T t50 = m(2,0) * m(3,3);
	T t51 = m(0,1) * m(1,2);
	T t53 = m(3,2) * m(1,0);
	T t56 = m(0,2) * m(2,1);
	T t58 = m(3,0) * m(0,3);
	T t63 = m(3,2) * m(2,0);
	T t64 = m(0,1) * m(1,3);
	T t66 = m(1,0) * m(0,3);
	T t68 = -t7 * t33 - t45 * t23 - t47 * m(0,1) * m(2,2) + t50 * t51 + t53 *
		m(0,1) * m(2,3) + t47 * t56 + t58 * t20 + t9 * t37 + t41 * t31 + t45 *
		t25 - t63 * t64 - t11 * t66;
	T t70 = (T)1.0 / (t43 + t68);
	T t72 = m(3,3) * m(0,1);
	T t74 = m(3,2) * m(0,1);
	T t78 = m(0,3) * m(3,1);
	T t108 = m(2,0) * m(1,2);
	T t111 = m(1,3) * m(3,0);
	T t131 = m(0,0) * m(1,2);
	T t135 = m(1,0) * m(0,2);
	T t148 = m(3,1) * m(2,0);
	T t150 = m(1,0) * m(2,1);
	T t156 = m(0,0) * m(2,1);
	T t158 = m(0,0) * m(2,3);
	T t161 = m(2,0) * m(0,1);
	im(0,0) = (t1 * m(2,2) - t3 * m(2,3) - m(3,3) * m(1,2) * m(2,1) + 
		t7 * m(2,3) - t9 * m(2,2) + t11 * m(1,3)) * t70;
	im(0,1) = -(t72 * m(2,2) - t74 * m(2,3) - t56 * m(3,3) + t35 * m(3,1) - 
		t78 * m(2,2) + m(0,3) * m(3,2) * m(2,1)) * t70;
	im(0,2) = (t72 * m(1,2) - t74 * m(1,3) - t1 * m(0,2) + m(0,2) * m(3,1) *
		m(1,3) + t3 * m(0,3) - t78 * m(1,2)) * t70;
	im(0,3) = -(t51 * m(2,3) - t64 * m(2,2) - m(1,1) * m(0,2) * m(2,3) + t56 *
		m(1,3) + m(1,1) * m(0,3) * m(2,2) - m(0,3) * m(1,2) * m(2,1)) * t70;
	im(1,0) = -(t47 * m(2,2) - t53 * m(2,3) + m(1,3) * m(3,2) * m(2,0) - t108 *
		m(3,3) + t23 * m(3,0) - t111 * m(2,2)) * t70;
	im(1,1) = (t19 * m(2,2) - t27 * m(2,3) - t58 * m(2,2) + t63 * m(0,3) + t39 *
		m(2,3) - t50 * m(0,2)) * t70;
	im(1,2) = -(t19 * m(1,2) - t27 * m(1,3) - t47 * m(0,2) - t58 * m(1,2) + t111 *
		m(0,2) + t66 * m(3,2)) * t70;
	im(1,3) = (t131 * m(2,3) - m(0,0) * m(1,3) * m(2,2) - t135 * m(2,3) - t108 *
		m(0,3) + m(1,3) * m(2,0) * m(0,2) + t66 * m(2,2)) * t70;
	im(2,0) = (-m(1,1) * m(2,0) * m(3,3) + m(1,1) * m(2,3) * m(3,0) - t28 *
		m(3,0) + t148 * m(1,3) + t150 * m(3,3) - m(2,3) * m(3,1) * m(1,0)) * t70;
	im(2,1) = -(t156 * m(3,3) - t158 * m(3,1) + t33 * m(3,1) - t161 * m(3,3) - m(2,1) *
		m(3,0) * m(0,3) + m(2,3) * m(3,0) * m(0,1)) * t70;
	im(2,2) = (t19 * m(1,1) - t22 * m(1,3) - t58 * m(1,1) - t47 * m(0,1) + t41 *
		m(0,3) + t45 * m(1,3)) * t70;
	im(2,3) = -(-m(2,3) * m(1,0) * m(0,1) + t158 * m(1,1) - t33 * m(1,1) + t161 *
		m(1,3) - t156 * m(1,3) + t150 * m(0,3)) * t70;
	im(3,0) = -(-t3 * m(2,0) + t30 * m(2,2) + t11 * m(1,0) - m(3,0) * m(1,2) *
		m(2,1) - t41 * m(2,2) + t7 * m(2,0)) * t70;
	im(3,1) = (-t22 * m(2,2) + t27 * m(2,1) - t39 * m(2,1) + t148 * m(0,2) + t45 *
		m(2,2) - t63 * m(0,1)) * t70;
	im(3,2) = -(-t53 * m(0,1) + t27 * m(1,1) - t39 * m(1,1) + t41 * m(0,2) - t22 *
		m(1,2) + t45 * m(1,2)) * t70;
	im(3,3) = t70 * (t161 * m(1,2) - t37 * m(1,1) - m(1,0) * m(0,1) * m(2,2) + t135 *
		m(2,1) + t14 * m(2,2) - t131 * m(2,1));

	return im;
}


template <typename T, int M, int N>
tiny_mat<T,M,N> operator*(int s,const tiny_mat<T,M,N>& m) 
{
	return m*s;
}

template <typename T, int M, int N>
tiny_mat<T,M,N> operator*(float s,const tiny_mat<T,M,N>& m)
{
	return m*s;
}

template <typename T, int M, int N>
tiny_mat<T,M,N> operator*(double s,const tiny_mat<T,M,N>& m) 
{
	return m*s;
}

template <typename T,int M, int N>
std::ostream& operator<<(std::ostream& out, const tiny_mat<T,M,N>& m)
{
	for(int i = 0; i < M; i++)
	{

		if(N > 0)
			out << m(i,0);
		for(int j = 1; j < N;j++)
			out << " "<<m(i,j);
		out << std::endl;
	}
	return out;
}

template <typename T, int M, int N>
tiny_mat<T,M,N> dyad(const tiny_vec<T,M>& v1, const tiny_vec<T,N>& v2)
{
	tiny_mat<T,M,N> m;
	for(int j = 0; j < N; j++)
		for(int i = 0; i < M; i++)
			m(i,j) = v1(i)*v2(j);
	return m;
}

#endif
