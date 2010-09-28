// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#ifndef VEC_H
#define VEC_H

#pragma	warning (disable: 4996)

#include <cmath>
#include <iostream>
#include <algorithm>
#include <functional>
#include <numeric>
#include <complex>
#include <cassert>

#include <iterator>




template <typename T>
class vec
{
	size_t _size;
	T*  _data;
public:
	typedef T              value_type;
	typedef T&             reference;
    typedef const T&       const_reference;
    typedef std::size_t    size_type;
	typedef std::ptrdiff_t difference_type;
	typedef T*             pointer;
	typedef const T*       const_pointer;
	typedef T* iterator;
	typedef const T* const_iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

	vec() : _size(0), _data(NULL)
	{
	}

	explicit vec(size_t n) : _size(n)
	{
		_data = new T[n];
	}

	template <typename InputIterator>
	vec(InputIterator first,InputIterator last):_size( last-first)
	{
		_data = new T[_size];
		std::copy(first,last,begin());
	}

	vec(size_t n, const T* marray):_size(n),_data(NULL)
	{
		if(_size > 0)
		{		
			_data = new T[_size];
			std::copy(marray,marray+_size,begin());
		}	
	}

	vec(const T& x,const T& y) : _size(2)
	{
		_data = new T[2];
		_data[0]=x;
		_data[1]=y;
	}

	vec(const T& x,const T& y,const T& z) : _size(3)
	{
		_data = new T[3];
		_data[0]=x;
		_data[1]=y;
		_data[2]=z;
	}

	vec(const T& x,const T& y,const T& z, const T& w) : _size(4)
	{
		_data = new T[4];
		_data[0]=x;
		_data[1]=y;
		_data[2]=z;
		_data[3]=w;
	}

	vec(const vec<T>& v)  
	{
		_size =v.size();
		_data = new T[_size];
		std::copy(v.begin(),v.end(),begin());
	}

#ifdef _HAS_CPP0X
	vec(vec<T> && v) : _size(0),_data(NULL)
	{
		std::swap(_size,v._size);
		std::swap(_data,v._data);
	}
#endif

	virtual ~vec()
	{
		if(_data)
		{
			delete[] _data;
			_data = NULL;
			_size=0;
		}
	}

	operator T*()
	{
		return _data;
	}

	operator const T*() const
	{
		return _data;
	}

	vec<T>& operator=(const T &s)
	{
#ifdef _HAS_CPP0X
		std::for_each(begin(),end(),[&s](T& elem) {elem=s;});
#else
		for(int i = 0; i < size(); i++)
			_data[i]=s;
#endif
		return *this;
	}

	vec<T>& operator=(const vec<T> & v)
	{
		if(&v== this) return *this;
		resize(v._size);
		std::copy(v.begin(),v.end(),begin());
		return *this;
	}
#ifdef _HAS_CPP0X
	vec<T>& operator=(vec<T> && v)
	{
		if(&v == this) return *this;
		std::swap(_data,v._data);
		std::swap(_size,v._size);
		return *this;
	}
#endif

	void resize(size_t n)
	{
		if(n == _size)
			return;
		
		if(_data)
		{
			delete[] _data;
			_data=NULL;
			_size=0;
		}
		if(n > 0)
		{
			_data=new T[n];
			_size=n;
		}			
	}

	

	void fill(const T& s)
	{
		std::fill(begin(),end(),s);
	}

	void zeros(size_t m)
	{
		resize(m);
		T zero;
		zero=0;
		std::fill(begin(), end(), zero);
	}
	void zeros()
	{	
		T zero;
		zero=0;
		std::fill(begin(), end(), zero);
	}

	void ones()
	{
		T one;
		one = 1;
		std::fill(begin(), end(), one);
	}

	void ones(size_t m)
	{
		resize(m);
		T one;
		one = 1;
		std::fill(begin(), end(), one);
	}

	iterator begin()
	{
		return _data;
	}
	const_iterator begin() const
	{
		return _data;
	}
	iterator end()
	{
		return _data + _size;
	}
	const_iterator end() const
	{
		return _data +_size;
	}
	reverse_iterator rbegin()
	{
		return reverse_iterator(end());
	}
	reverse_iterator rend()
	{
		return reverse_iterator(begin());
	}
	const_reverse_iterator rbegin() const
	{
		return const_reverse_iterator(end());
	}
	const_reverse_iterator rend() const
	{
		return const_reverse_iterator(begin());
	}
	size_t size() const
	{
		return _size;
	}
	bool operator==(const vec& v) const
	{
		
		return std::equal(begin(),end(),v.begin());
	}

	bool operator!=(const vec& v) const
	{
		return !operator==(v);
	}

	vec operator-() const
	{
		return  (T)-1* *this;
	}


	vec& operator+=(const T& s)
	{
#ifdef _HAS_CPP0X
		std::for_each(begin(),end(),[&s](T& elem){elem+=s;});
#else
		for(int i = 0; i < size(); i++)
			_data[i]+= s;
#endif
		return *this;
	}

	vec operator+(const T& s) const
	{
		vec r = *this;
		r += s;
		return r;
	}

	vec& operator-=(const T& s)
	{
#ifdef _HAS_CPP0X
		std::for_each(begin(),end(),[&s](T& elem){elem-=s;});
#else
		for(int i = 0; i < size(); i++)
			_data[i]-=s;
#endif
		return *this;
	}
	vec operator-(const T& s) const
	{
		vec r = *this;
		r -= s;
		return r;
	}

	vec& operator*=(const T& s)
	{
#ifdef _HAS_CPP0X
		std::for_each(begin(),end(),[&s](T& elem){elem *= s;});
#else
		for(int i = 0; i < size(); i++)
			 _data[i]*=s;
#endif
		return *this;
	}

	vec operator*(const T& s) const
	{
		vec r = *this;
		r *= s;
		return r;
	}

	vec& operator/=(const T& s)
	{
#ifdef _HAS_CPP0X
		std::for_each(begin(),end(),[&s](T& elem){elem/=s;});
#else
		for(int i = 0; i < size(); i++)
			_data[i]/= s;
#endif
		return *this;
	}

	vec operator/(const T& s) const
	{
		vec r = *this;
		r /= s;
		return r;
	}


	vec& operator+=(const vec& v)
	{
#ifdef _HAS_CPP0X
		vec::const_iterator vit=v.begin();
		std::for_each(begin(),end(),[&vit](T& elem){elem+=*vit++;});
#else
		for(int i = 0; i < (int)_size; i++)
			_data[i] += v[i]; 
#endif
		return *this;
	}

	vec operator+(const vec& v) const
	{
		vec r = *this;
		r += v;
		return r;
	}

	vec& operator-=(const vec& v)
	{
#ifdef _HAS_CPP0X
		vec::const_iterator vit=v.begin();
		std::for_each(begin(),end(),[&vit](T& elem){elem-=*vit++;});
#else
		for(int i = 0; i < size(); i++)
			_data[i] -= v[i]; 
#endif
		return *this;
	}
	vec operator-(const vec& v) const
	{
		vec r = *this;
		r -= v;
		return r;
	}

	vec& operator*=(const vec& v)
	{
#ifdef _HAS_CPP0X
		vec::const_iterator vit=v.begin();
		std::for_each(begin(),end(),[&vit](T& elem){elem*=*vit++;});
#else
		for(int i = 0; i < size(); i++)
			_data[i] *= v[i]; 
#endif
		return *this;
	}

	vec operator*(const vec& v) const
	{
		vec r = *this;
		r *= v;
		return r;
	}

	vec& operator/=(const vec& v)
	{
#ifdef _HAS_CPP0X
		vec::iterator vit=v.begin();
		std::for_each(begin(),end(),[&vit](T& elem){elem/=*vit++;});
#else
		for(int i = 0; i < size(); i++)
			_data[i] /= v[i]; 
#endif

		return *this;
	}

	vec operator/(const vec& v) const
	{
		vec r = *this;
		r /= v;
		return r;
	}

	

	T& operator()(size_t i)
	{
		assert(i >= 0 && i < _size);
		return _data[i];
	}
	
	const T& operator()(size_t i) const
	{
		assert(i >= 0 && i < _size);
		return _data[i];
	}


	void sort()
	{
		std::sort(begin(),end());
	}

	void random_shuffle()
	{
		std::random_shuffle(begin(),end());
	}

	T& min_elem()
	{
		return *std::min_element(begin(),end());
	}

	size_t min_index()
	{
		return std::distance(begin(),std::min_element(begin(),end()));
	}

	T& max_elem()
	{
		return *std::max_element(begin(),end());
	}

	size_t max_index()
	{
		return std::distance(begin(),std::max_element(begin(),end()));
	}

	T& median_elem()
	{
		vec::iterator it= begin() + _size/2;
		std::nth_element(begin(),it,end());
		return *it;
	}

	size_t median_index()
	{
		return begin() +_size/2;
	}

	T sqr_length() const
	{
		vec::const_iterator it= begin();
		T l = 0;
		while(it != end())
			l += (*it)*(*it++);
		
		return l;
	}

	T length() const
	{
		T l = sqr_length();
		return sqrt(l);
	}

};

template <typename T>
vec<T> operator+(const T& s, const vec<T>& v)
{
	return v+s;
}

template <typename T>
vec<T> operator-(const T& s, const vec<T>& v)
{
	return -v+s;
}


template <typename T>
vec<T> operator*(const T& s, const vec<T>& v)
{
	return v*s;
}

template <typename T>
vec<T> operator/(const T& s, const vec<T>& v)
{
	return v*(T)(1.0/s);
}

template <typename T>
T dot(const vec<T>& v1, const vec<T>& v2)
{
	return std::inner_product(v1.begin(),v1.end(),v2.begin(),(T)0);
}

template <typename T>
vec<T> cross(const vec<T>& b, const vec<T>& c)
{
	vec<T> a(3);
	a[0] = b(1)*c(2) - b(2)*c(1);
	a[1] = b(2)*c(0) - b(0)*c(2);
	a[2] = b(0)*c(1) - b(1)*c(0);
	return a;
}

template <typename T>
vec<T> homog(const vec<T>& v)
{
	vec<T> vh(v.size()+1);
	std::copy(v.begin(),v.end(),vh.begin());
	vh(v.size()) = (T)1;
	return vh;
}

template <typename T>
vec<T> unhomog(const vec<T>& v)
{
	vec<T> vh(v.size()-1);
	T w = *(v.end()-1);
#ifdef _HAS_CPP0X
	
	auto vit=v.begin();
	std::for_each(vh.begin(),vh.end(),[&](T& elem){elem=*vit++/w;});
#else
	for(int i = 0; i < (int)vh.size(); i++)
		vh[i] = v[i]/w;
#endif
	return vh;
}


template <typename T>
std::ostream& operator<<(std::ostream& out, const vec<T>& v)
{
#ifdef _HAS_CPP0X
	auto it = v.begin();
	if(it != v.end())
		out << *it ++; 
	std::for_each(it, v.end(),[&out](const T& elem){ out << " " << elem;});
#else
	if(v.size() > 0)
		out << v[0];
	for( int i = 1; i < (int)v.size(); i++)
		out << " " <<v[i];
#endif
	return out;
}

template <typename T>
std::istream& operator>>(std::istream& in, vec<T>& v)
{
#ifdef _HAS_CPP0X
	std::for_each(v.begin(), v.end(),[&in](T& elem){ in >> elem;});
#else
	for(int i =0; i < v.size(); i++)
		in >> v[i];
#endif
	return in;
}



///create a linearly spaced vector with N values starting at first_val and ending ant last_val
template <typename T>
const vec<T>  lin_space(const T& first_val, const T& last_val, unsigned N=10)
{
	vec<T> lv(N);
	if(N == 1)
	{
		lv(0) = last_val;
		return lv;
	}
	T diff = last_val-first_val;

	for(unsigned i = 0; i < N; i++)
	{
		lv(i) = first_val + i*diff/((T)N-(T)1.0);
	}
	return lv;
}



#endif
