#ifndef TINY_VEC_H
#define TINY_VEC_H
#include <iostream>
#include <algorithm>
#include "vec.h"
#include "math_utils.h"


template <typename T,int N>
class tiny_vec
{
	
	T  _data[N];

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

	tiny_vec();

	template <typename InputIterator>
	tiny_vec(InputIterator first,InputIterator last)
	{
		std::copy(first,last,begin());
	}

	tiny_vec(const vec<T>& v)
	{
		std::copy(v.begin(),v.end(),begin());
	}
	
	tiny_vec(size_t n, const T* marray);
	
	~tiny_vec();

	tiny_vec(const T&x,const T&y);
	
	tiny_vec(const T&x,const T&y, const T&z);
	
	tiny_vec(const T&x, const T&y, const T&z,const T& w);	

	int size() const;
	
	operator T*();
	
	operator vec<T>();
	
	operator const T*() const;

	void set(const T& x);

	void set(const T& x, const T& y);
	
	void set(const T& x, const T& y,const T& z);
	
	void set(const T& x, const T& y,const T& z, const T& w);
	
	tiny_vec& operator=(const T &s);

	tiny_vec& operator=(int s);

	tiny_vec& operator=(const tiny_vec & v);
	
	tiny_vec& operator=(const vec<T> & v);		

	void fill(const T& s);

	void zeros(size_t m);
	
	void zeros();

	void ones();
	
	void ones(size_t m);

	iterator begin();
	
	iterator end();
	
	const_iterator begin() const;

	const_iterator end() const;
	
	reverse_iterator rbegin();
	
	reverse_iterator rend();
	
	const_reverse_iterator rbegin() const;
	
	const_reverse_iterator rend() const;	

	bool operator==(const tiny_vec& v) const;

	bool operator==(const vec<T>& v) const;

	bool operator!=(const tiny_vec& v) const;
	
	bool operator!=(const vec<T>& v) const;

	tiny_vec operator-() const;
	


	tiny_vec& operator+=(int s);

	tiny_vec& operator+=(float s);

	tiny_vec& operator+=(double s);


	
	tiny_vec operator+(int s) const;

	tiny_vec operator+(float s) const;

	tiny_vec operator+(double s) const;
	
	
	
	tiny_vec& operator-=(int s);

	tiny_vec& operator-=(float s);
		
	tiny_vec& operator-=(double s);
	

	
	tiny_vec operator-(int s) const;

	tiny_vec operator-(float s) const;

	tiny_vec operator-(double s) const;
	
	
	
	tiny_vec& operator*=(int s);

	tiny_vec& operator*=(float s);

	tiny_vec& operator*=(double s);
	

		
	tiny_vec operator*(int s) const;

	tiny_vec operator*(float s) const;

	tiny_vec operator*(double s) const;
	


	tiny_vec& operator/=(int s);

	tiny_vec& operator/=(float s);

	tiny_vec& operator/=(double s);


	
	tiny_vec operator/(int s) const;

	tiny_vec operator/(float s) const;
	
	tiny_vec operator/(double s) const;
	
	tiny_vec& operator+=(const tiny_vec& v);
	
	tiny_vec& operator+=(const vec<T>& v);
	
	tiny_vec operator+(const tiny_vec& v) const;

	tiny_vec operator+(const vec<T>& v) const;
	
	tiny_vec& operator-=(const tiny_vec& v);

	tiny_vec& operator-=(const vec<T>& v);
	
	tiny_vec operator-(const tiny_vec& v) const;

	tiny_vec operator-(const vec<T>& v) const;

	tiny_vec& operator*=(const tiny_vec& v);
	
	tiny_vec& operator*=(const vec<T>& v);
	
	tiny_vec operator*(const tiny_vec& v) const;
	
	tiny_vec operator*(const vec<T>& v) const;
	
	tiny_vec& operator/=(const tiny_vec& v);
	
	tiny_vec& operator/=(const vec<T>& v);
	
	tiny_vec operator/(const tiny_vec& v) const;
	
	tiny_vec operator/(const vec<T>& v) const;

	T& operator()(size_t i);
	
	T operator()(size_t i) const;
	
	void sort();

	void random_shuffle();
	
	T& min_elem();
	
	size_t min_index();
	
	T& max_elem();
	
	size_t max_index();
	
	T& median_elem();
	
	size_t median_index();
	
	T sqr_length() const;
	
	T length() const;

	void clamp(const T& lo, const T& hi);

	void normalize();
	
};



template <typename T, int N>
tiny_vec<T,N>::tiny_vec(){}


template <typename T, int N>
tiny_vec<T,N>::tiny_vec(size_t n, const T* marray)
{			
	std::copy(marray,marray+_size,begin());		
}

template <typename T, int N>
tiny_vec<T,N>::~tiny_vec(){}

template <typename T, int N>
tiny_vec<T,N>::tiny_vec(const T&x,const T&y)
{
	_data[0]=x;
	_data[1]=y;
}

template <typename T, int N>
tiny_vec<T,N>::tiny_vec(const T&x,const T&y, const T&z)
{
	_data[0]=x;
	_data[1]=y;
	_data[2]=z;
}
	
template <typename T, int N>
tiny_vec<T,N>::tiny_vec(const T&x, const T&y, const T&z,const T& w)
{
	_data[0]=x;
	_data[1]=y;
	_data[2]=z;
	_data[3]=w;
}

template <typename T, int N>
void tiny_vec<T,N>::set(const T& x)
{
	_data[0]=x;
}

template <typename T, int N>
void tiny_vec<T,N>::set(const T& x, const T& y)
{
	_data[0]=x;
	_data[1]=y;

}
template <typename T, int N>
void tiny_vec<T,N>::set(const T& x, const T& y,const T& z)
{
	_data[0]=x;
	_data[1]=y;
	_data[2]=z;
	
}
template <typename T, int N>
void tiny_vec<T,N>::set(const T& x, const T& y,const T& z, const T& w)
{
	_data[0]=x;
	_data[1]=y;
	_data[2]=z;
	_data[3]=w;
}

template <typename T, int N>
int tiny_vec<T,N>::size() const
{
	return N;
}

	template <typename T, int N>
	tiny_vec<T,N>::operator T*()
	{
		return _data;
	}

	template <typename T, int N>
	tiny_vec<T,N>::operator vec<T>()
	{
		vec<T> v(begin(),end());
		return v;
	}

	template <typename T, int N>
	tiny_vec<T,N>::operator const T*() const
	{
		return _data;
	}

	template <typename T, int N>
	tiny_vec<T,N>& tiny_vec<T,N>::operator=(const T &s)
	{
#ifdef _HAS_CPP0X
		std::for_each(begin(),end(),[&s](T& elem) {elem=s;});
#else
		for(int i=0;i < N;i++)
			_data[i]=s;
#endif
		return *this;
	}

	template <typename T, int N>
	tiny_vec<T,N>& tiny_vec<T,N>::operator=(int s)
	{
#ifdef _HAS_CPP0X
		std::for_each(begin(),end(),[&s](T& elem) {elem=(T)s;});
#else
		for(int i=0;i<N;i++)
			_data[i]=(T)s;
#endif
		return *this;
	}

	template <typename T, int N>
	tiny_vec<T,N>& tiny_vec<T,N>::operator=(const tiny_vec & v)
	{
		if(&v== this) return *this;
		
		std::copy(v.begin(),v.end(),begin());
		return *this;
	}

	template <typename T, int N>
	tiny_vec<T,N>& tiny_vec<T,N>::operator=(const vec<T> & v)
	{
		
		std::copy(v.begin(),v.end(),begin());
		return *this;
	}


		
	template <typename T, int N>
	void tiny_vec<T,N>::fill(const T& s)
	{
		std::fill(begin(),end(),s);
	}

	template <typename T, int N>
	void tiny_vec<T,N>::zeros(size_t m)
	{
		T zero;
		zero=0;
		std::fill(begin(), end(), zero);
	}

template <typename T, int N>
void tiny_vec<T,N>::zeros()
{	
	T zero;
	zero=0;
	std::fill(begin(), end(), zero);
}

template <typename T, int N>
void tiny_vec<T,N>::ones()
{
	T one;
	one = 1;
	std::fill(begin(), end(), one);
}

template <typename T, int N>
void tiny_vec<T,N>::ones(size_t m)
{
	resize(m);
	T one;
	one = 1;
	std::fill(begin(), end(), one);
}

template <typename T, int N>
typename tiny_vec<T,N>::iterator tiny_vec<T,N>::begin()
{
	return _data;
}

template <typename T, int N>
typename tiny_vec<T,N>::const_iterator tiny_vec<T,N>::begin() const
{
	return _data;
}

template <typename T, int N>
typename tiny_vec<T,N>::iterator tiny_vec<T,N>::end()
{
	return _data + N;
}

template <typename T, int N>
typename tiny_vec<T,N>::const_iterator tiny_vec<T,N>::end() const
{
	return _data +N;
}

template <typename T, int N>
typename tiny_vec<T,N>::reverse_iterator tiny_vec<T,N>::rbegin()
{
	return reverse_iterator(end());
}

template <typename T, int N>
typename tiny_vec<T,N>::reverse_iterator tiny_vec<T,N>::rend()
{
	return reverse_iterator(begin());
}

template <typename T, int N>
typename tiny_vec<T,N>::const_reverse_iterator tiny_vec<T,N>::rbegin() const
{
	return const_reverse_iterator(end());
}

template <typename T, int N>
typename tiny_vec<T,N>::const_reverse_iterator tiny_vec<T,N>:: rend() const
{
	return const_reverse_iterator(begin());
}

template <typename T, int N>
bool tiny_vec<T,N>::operator==(const tiny_vec<T,N>& v) const
{
		
	return std::equal(begin(),end(),v.begin());
}


template <typename T, int N>
bool tiny_vec<T,N>::operator==(const vec<T>& v) const
{		
	return std::equal(begin(),end(),v.begin());
}


template <typename T, int N>
bool tiny_vec<T,N>::operator!=(const tiny_vec<T,N>& v) const
{
	return !(*this == v);
}

template <typename T, int N>
bool tiny_vec<T,N>::operator!=(const vec<T>& v) const
{
	return !(*this==v);
}

template <typename T, int N>
tiny_vec<T,N> tiny_vec<T,N>::operator-() const
{
	return  (T)-1* *this;
}


template <typename T, int N>
tiny_vec<T,N>& tiny_vec<T,N>::operator+=(int s)
{
#ifdef _HAS_CPP0X
	std::for_each(begin(),end(),[&s](T& elem){elem+=(T)s;});
#else
	for(int i=0;i<size();i++)
		_data[i]+=s;
#endif
	return *this;
}

template <typename T, int N>
tiny_vec<T,N>& tiny_vec<T,N>::operator+=(float s)
{
#ifdef _HAS_CPP0X
	std::for_each(begin(),end(),[&s](T& elem){elem=(T)((float)elem+s);});
#else
	for(int i=0;i<size();i++)
		_data[i]=(T)((float)_data[i]+s);
#endif
	return *this;
}

template <typename T, int N>
tiny_vec<T,N>& tiny_vec<T,N>::operator+=(double s)
{
#ifdef _HAS_CPP0X
	std::for_each(begin(),end(),[&s](T& elem){elem=(T) ((double)elem+s);});
#else
	for(int i=0;i<size();i++)
		_data[i]=(T)((double)_data[i]+s);
#endif
	return *this;
}


template <typename T, int N>
tiny_vec<T,N> tiny_vec<T,N>::operator+(int s) const
{
	tiny_vec r = *this;
	r += s;
	return r;
}

template <typename T, int N>
tiny_vec<T,N> tiny_vec<T,N>::operator+(float s) const
{
	tiny_vec r = *this;
	r += s;
	return r;
}

template <typename T, int N>
tiny_vec<T,N> tiny_vec<T,N>::operator+(double s) const
{
	tiny_vec r = *this;
	r += s;
	return r;
}


template <typename T, int N>
tiny_vec<T,N>& tiny_vec<T,N>::operator-=(int s)
{
	
#ifdef _HAS_CPP0X
	std::for_each(begin(),end(),[&s](T& elem){elem-=(T)s;});
#else
	for(int i=0;i<N;i++)
		_data[i]-=(T)s;
#endif
	return *this;
}

template <typename T, int N>
tiny_vec<T,N>& tiny_vec<T,N>::operator-=(float s)
{
	
#ifdef _HAS_CPP0X
	std::for_each(begin(),end(),[&s](T& elem){elem=(T)((float)elem-s);});
#else
	for(int i=0;i<N;i++)
		_data[i]=(T)((float)_data[i]-s);
#endif
	return *this;
}


template <typename T, int N>
tiny_vec<T,N>& tiny_vec<T,N>::operator-=(double s)
{
	
#ifdef _HAS_CPP0X
	std::for_each(begin(),end(),[&s](T& elem){elem=(T)((double)elem-s);});
#else
	for(int i=0;i<N;i++)
		_data[i]=(T)((double)_data[i]-s);
#endif
	return *this;
}


template <typename T, int N>
tiny_vec<T,N> tiny_vec<T,N>::operator-(int s) const
{
	tiny_vec r = *this;
	r -= (T)s;
	return r;
}

template <typename T, int N>
tiny_vec<T,N> tiny_vec<T,N>::operator-(float s) const
{
	tiny_vec r = *this;
	r -= (T)s;
	return r;
}


template <typename T, int N>
tiny_vec<T,N> tiny_vec<T,N>::operator-(double s) const
{
	tiny_vec r = *this;
	r -= (T)s;
	return r;
}



template <typename T, int N>
tiny_vec<T,N>& tiny_vec<T,N>::operator*=(int s)
{
#ifdef _HAS_CPP0X
	std::for_each(begin(),end(),[&s](T& elem){elem *= (T)s;});
#else
	for(int i=0; i <N; i++)
		_data[i]*=(T)s;
#endif
	return *this;
}

template <typename T, int N>
tiny_vec<T,N>& tiny_vec<T,N>::operator*=(float s)
{
#ifdef _HAS_CPP0X
	std::for_each(begin(),end(),[&s](T& elem){elem =(T) ((float)elem *s);});
#else
	for(int i=0; i <N; i++)
		_data[i]= (T)((float)_data[i]*s);
#endif
	return *this;
}


template <typename T, int N>
tiny_vec<T,N>& tiny_vec<T,N>::operator*=(double s)
{
#ifdef _HAS_CPP0X
	std::for_each(begin(),end(),[&s](T& elem){elem =(T)((double)elem* s);});
#else
	for(int i=0; i <N; i++)
		_data[i]=(T)((double)_data[i]*s);
#endif
	return *this;
}

	
		
template <typename T, int N>
tiny_vec<T,N> tiny_vec<T,N>::operator*(int s) const
{
	tiny_vec<T,N> r = *this;
	r *= s;
	return r;
}

template <typename T, int N>
tiny_vec<T,N> tiny_vec<T,N>::operator*(float s) const
{
	tiny_vec<T,N> r = *this;
	r *= s;
	return r;
}


template <typename T, int N>
tiny_vec<T,N> tiny_vec<T,N>::operator*(double s) const
{
	tiny_vec<T,N> r = *this;
	r *= s;
	return r;
}


template <typename T, int N>
tiny_vec<T,N>& tiny_vec<T,N>::operator/=(int s)
{
#ifdef _HAS_CPP0X
	std::for_each(begin(),end(),[&s](T& elem){elem/=(T)s;});
#else
	for(int i=0;i <N; i++)
		_data[i]/=(T)s;
#endif
	return *this;
}

template <typename T, int N>
tiny_vec<T,N>& tiny_vec<T,N>::operator/=(float s)
{
#ifdef _HAS_CPP0X
	std::for_each(begin(),end(),[&s](T& elem){elem=(T)(elem/s);});
#else
	for(int i=0;i <N; i++)
		_data[i]=(T)((float)_data[i]/s);
#endif
	return *this;
}


template <typename T, int N>
tiny_vec<T,N>& tiny_vec<T,N>::operator/=(double s)
{
#ifdef _HAS_CPP0X
	std::for_each(begin(),end(),[&s](T& elem){elem=(T)((double)elem/s);});
#else
	for(int i=0;i <N; i++)
		_data[i]=(T)((double)_data[i]/s);
#endif
	return *this;
}


template <typename T, int N>
tiny_vec<T,N> tiny_vec<T,N>::operator/(int s) const
{
	tiny_vec<T,N> r = *this;
	r /= s;
	return r;
}

template <typename T, int N>
tiny_vec<T,N> tiny_vec<T,N>::operator/(float s) const
{
	tiny_vec<T,N> r = *this;
	r /= s;
	return r;
}


template <typename T, int N>
tiny_vec<T,N> tiny_vec<T,N>::operator/(double s) const
{
	tiny_vec<T,N> r = *this;
	r /= s;
	return r;
}

template <typename T, int N>
tiny_vec<T,N>& tiny_vec<T,N>::operator+=(const tiny_vec& v)
{
#ifdef _HAS_CPP0X
	const_iterator vit=v.begin();
	std::for_each(begin(),end(),[&vit](T& elem){elem+=*vit++;});
#else
	for(int i=0; i <N;i++)
		_data[i] += v[i];
#endif
	return *this;
}

template <typename T, int N>
tiny_vec<T,N>& tiny_vec<T,N>::operator+=(const vec<T>& v)
{
#ifdef _HAS_CPP0X
	const_iterator vit=v.begin();
	std::for_each(begin(),end(),[&vit](T& elem){elem+=*vit++;});
#else
	for(int i=0; i <N; i++)
		_data[i] += v[i];
#endif
	return *this;
}

template <typename T, int N>
tiny_vec<T,N> tiny_vec<T,N>::operator+(const tiny_vec& v) const
{
	tiny_vec r = *this;
	r += v;
	return r;
}

template <typename T, int N>
tiny_vec<T,N> tiny_vec<T,N>::operator+(const vec<T>& v) const
{
	tiny_vec<T,N> r = *this;
	r += v;
	return r;
}

template <typename T, int N>
tiny_vec<T,N>& tiny_vec<T,N>::operator-=(const tiny_vec& v)
{
#ifdef _HAS_CPP0X
	const_iterator vit=v.begin();
	std::for_each(begin(),end(),[&vit](T& elem){elem-=*vit++;});
#else
	for(int i =0; i < N; i++)
		_data[i]-= v[i];
#endif
	return *this;
}

template <typename T, int N>
tiny_vec<T,N>& tiny_vec<T,N>::operator-=(const vec<T>& v)
{
#ifdef _HAS_CPP0X
	const_iterator vit=v.begin();
	std::for_each(begin(),end(),[&vit](T& elem){elem-=*vit++;});
#else
	for(int i =0; i < N; i++)
		_data[i]-= v[i];
#endif
	return *this;
}

template <typename T, int N>
tiny_vec<T,N> tiny_vec<T,N>::operator-(const tiny_vec& v) const
{
	tiny_vec r = *this;
	r -= v;
	return r;
}

template <typename T, int N>
tiny_vec<T,N> tiny_vec<T,N>::operator-(const vec<T>& v) const
{
	tiny_vec r = *this;
	r -= v;
	return r;
}

template <typename T, int N>
tiny_vec<T,N>& tiny_vec<T,N>::operator*=(const tiny_vec& v)
{
#ifdef _HAS_CPP0X
	auto vit=v.begin();
	std::for_each(begin(),end(),[&vit](T& elem){elem*=*vit++;});
#else
	for(int i=0; i < N; i++)
		_data[i]*= v[i];
#endif
	return *this;
}

template <typename T, int N>
tiny_vec<T,N>& tiny_vec<T,N>::operator*=(const vec<T>& v)
{
#ifdef _HAS_CPP0X
	auto vit=v.begin();
	std::for_each(begin(),end(),[&vit](T& elem){elem*=*vit++;});
#else
	for(int i=0; i < N; i++)
		_data[i]*= v[i];
#endif

	return *this;
}

template <typename T, int N>
tiny_vec<T,N> tiny_vec<T,N>::operator*(const tiny_vec& v) const
{
	tiny_vec<T,N> r = *this;
	r *= v;
	return r;
}

template <typename T, int N>
tiny_vec<T,N> tiny_vec<T,N>::operator*(const vec<T>& v) const
{
	tiny_vec r = *this;
	r *= v;
	return r;
}

template <typename T, int N>
tiny_vec<T,N>& tiny_vec<T,N>::operator/=(const tiny_vec& v)
{
#ifdef _HAS_CPP0X
	auto vit=v.begin();
	std::for_each(begin(),end(),[&vit](T& elem){elem/=*vit++;});
#else
	for(int i = 0; i < N; i++)
		_data[i] /= v[i];
#endif
	return *this;
}

template <typename T, int N>
tiny_vec<T,N>& tiny_vec<T,N>::operator/=(const vec<T>& v)
{
#ifdef _HAS_CPP0X
	auto vit=v.begin();
	std::for_each(begin(),end(),[&vit](T& elem){elem/=*vit++;});
#else
	for(int i = 0; i < N; i++)
		_data[i]/= v[i];
#endif
	return *this;
}

template <typename T, int N>
tiny_vec<T,N> tiny_vec<T,N>::operator/(const tiny_vec<T,N>& v) const
{
	tiny_vec r = *this;
	r /= v;
	return r;
}

template <typename T, int N>
tiny_vec<T,N> tiny_vec<T,N>::operator/(const vec<T>& v) const
{
	tiny_vec r = *this;
	r /= v;
	return r;
}

template <typename T, int N>
T& tiny_vec<T,N>::operator()(size_t i)
{
	assert(i >= 0 && i < N);
	return _data[i];
}
	
template <typename T, int N>
T tiny_vec<T,N>::operator()(size_t i) const
{
	assert(i >= 0 && i < N);
	return _data[i];
}

template <typename T, int N>
void tiny_vec<T,N>::sort()
{
	std::sort(begin(),end());
}

template <typename T, int N>
void tiny_vec<T,N>::random_shuffle()
{
	std::random_shuffle(begin(),end());
}

template <typename T, int N>
T& tiny_vec<T,N>::min_elem()
{
	return *std::min_element(begin(),end());
}

template <typename T, int N>
size_t tiny_vec<T,N>::min_index()
{
	return std::distance(begin(),std::min_element(begin(),end()));
}

template <typename T, int N>
T& tiny_vec<T,N>::max_elem()
{
	return *std::max_element(begin(),end());
}

template <typename T, int N>
size_t tiny_vec<T,N>::max_index()
{
	return std::distance(begin(),std::max_element(begin(),end()));
}

template <typename T, int N>
T& tiny_vec<T,N>::median_elem()
{
	iterator it= begin() + _size/2;
	std::nth_element(begin(),it,end());
	return *it;
}

template <typename T, int N>
size_t tiny_vec<T,N>::median_index()
{
	return begin() +_size/2;
}

template <typename T, int N>
T tiny_vec<T,N>::sqr_length() const
{
	const_iterator it= begin();
	T l = 0;
	while(it != end())
		l += (*it)*(*it++);
		
	return l;
}

template <typename T, int N>
T tiny_vec<T,N>::length() const 
{
		T l = sqr_length();
		return sqrt(l);
}




template <typename T, int N>
tiny_vec<T,N> operator+(int s, const tiny_vec<T,N>& v)
{
	return v+s;
}

template <typename T, int N>
tiny_vec<T,N> operator+(float s, const tiny_vec<T,N>& v)
{
	return v+s;
}


template <typename T, int N>
tiny_vec<T,N> operator+(double s, const tiny_vec<T,N>& v)
{
	return v+s;
}



template <typename T, int N>
tiny_vec<T,N> operator-(int s, const tiny_vec<T,N>& v)
{
	return -v+(T)s;
}

template <typename T, int N>
tiny_vec<T,N> operator-(float s, const tiny_vec<T,N>& v)
{
	return -v+s;
}

template <typename T, int N>
tiny_vec<T,N> operator-(double s, const tiny_vec<T,N>& v)
{
	return -v+s;
}


template <typename T, int N>
tiny_vec<T,N> operator*(int s, const tiny_vec<T,N>& v)
{
	return v*(T)s;
}

template <typename T, int N>
tiny_vec<T,N> operator*(float s, const tiny_vec<T,N>& v)
{
	return v*s;
}

template <typename T, int N>
tiny_vec<T,N> operator*(double s, const tiny_vec<T,N>& v)
{
	return v*s;
}


template <typename T, int N>
tiny_vec<T,N> operator/(int s, const tiny_vec<T,N>& v)
{
	return v*(1.0/s);
}

template <typename T, int N>
tiny_vec<T,N> operator/(float s, const tiny_vec<T,N>& v)
{
	return v*(1.0f/s);
}

template <typename T, int N>
tiny_vec<T,N> operator/(double s, const tiny_vec<T,N>& v)
{
	return v*(1.0/s);
}

template <typename T, int N>
T dot(const tiny_vec<T,N>& v1, const tiny_vec<T,N>& v2)
{
	return std::inner_product(v1.begin(),v1.end(),v2.begin(),(T)0);
}


template <typename T, int N>
T dot(const tiny_vec<T,N>& v1, const vec<T>& v2)
{
	return std::inner_product(v1.begin(),v1.end(),v2.begin(),(T)0);
}

template <typename T, int N>
T dot(const vec<T>& v1, const tiny_vec<T,N>& v2)
{
	return std::inner_product(v1.begin(),v1.end(),v2.begin(),(T)0);
}



template <typename T>
tiny_vec<T,3> cross(const tiny_vec<T,3>& b, const tiny_vec<T,3>& c)
{
	
	tiny_vec<T,3> a;
	a[0] = b(1)*c(2) - b(2)*c(1);
	a[1] = b(2)*c(0) - b(0)*c(2);
	a[2] = b(0)*c(1) - b(1)*c(0);
	return a;
}

template <typename T>
vec<T> cross(const vec<T>& b, const tiny_vec<T,3>& c)
{

	vec<T> a(3);
	a[0] = b(1)*c(2) - b(2)*c(1);
	a[1] = b(2)*c(0) - b(0)*c(2);
	a[2] = b(0)*c(1) - b(1)*c(0);
	return a;
}

template <typename T>
tiny_vec<T,3> cross(const tiny_vec<T,3>& b, const vec<T>& c)
{

	tiny_vec<T,3> a;
	a[0] = b(1)*c(2) - b(2)*c(1);
	a[1] = b(2)*c(0) - b(0)*c(2);
	a[2] = b(0)*c(1) - b(1)*c(0);
	return a;
}


template <typename T, int N>
tiny_vec<T,N+1> homog(const tiny_vec<T,N>& v)
{
	tiny_vec<T,N+1> vh;
	std::copy(v.begin(),v.end(),vh.begin());
	vh(v.size()) = (T)1;
	return vh;
}

template <typename T, int N>
tiny_vec<T,N-1> unhomog(const tiny_vec<T,N>& v)
{
	tiny_vec<T,N-1> vh;
	T w = *(v.end()-1);
#ifdef _HAS_CPP0X
	auto vit=v.begin();
	std::for_each(vh.begin(),vh.end(),[&](T& elem){elem=*vit++/w;});
#else
	for(int i = 0; i < N-1; i++)
		vh[i] = v[i]/w;
#endif
	return vh;
}

template <typename T, int N>
void tiny_vec<T,N>::clamp(const T& lo, const T& hi)
{
#ifdef _HAS_CPP0X
	std::for_each(begin(),end(),[&](T& elem){elem =::clamp(elem,lo,hi);});
#else
	for(int i = 0; i < N-1; i++)
		_data[i] =::clamp(_data[i],lo,hi);
#endif
	
}




template <typename T, int N>
std::ostream& operator<<(std::ostream& out, const tiny_vec<T,N>& v)
{
#ifdef _HAS_CPP0X
	auto it = v.begin();
	if(it != v.end())
		out << *it ++; 
	std::for_each(it, v.end(),[&out](const T& elem){ out << " " << elem;});
#else
	if(v.size() > 0)
		out << v[0];
	for(int i = 1; i < v.size(); i++)
		out <<" " <<v[i];
#endif
	return out;
}


template <int N>
std::ostream& operator<<(std::ostream& out, const tiny_vec<unsigned char,N>& v)
{
#ifdef _HAS_CPP0X
	auto it = v.begin();
	if(it != v.end())
		out << (int)*it ++; 
	std::for_each(it, v.end(),[&out](const unsigned char& elem){ out << " " << (int)elem;});
#else
	if(v.size() > 0)
		out << (int)v[0];
	for(int i = 1; i < v.size(); i++)
		out <<" " <<(int)v[i];
#endif
	return out;
}


template <typename T,int N>
std::istream& operator>>(std::istream& in, tiny_vec<T,N>& v)
{
#ifdef _HAS_CPP0X
	std::for_each(v.begin(), v.end(),[&in](T& elem){ in >> elem;});
#else
	for(int i = 0; i < v.size(); i++)
		in >> v[i];
#endif
	return in;
}

template <typename T, int N>
void tiny_vec<T,N>::normalize()
{
	T l = length();
	if(l != 0)
		operator/=(l);
}


inline unsigned char rgb_2_gray(const tiny_vec<unsigned char,3>& rgb)
{	
	return (unsigned char)(0.2989f*rgb[0] + 0.5870f*rgb[1] +  0.1140f*rgb[2]);
}


inline float rgb_2_gray(const tiny_vec<float,3>& rgb)
{	
	return 0.2989f*rgb[0] + 0.5870f*rgb[1] +  0.1140f*rgb[2];
}


inline double rgb_2_gray(const tiny_vec<double,3>& rgb)
{	
	return 0.2989*rgb[0] + 0.5870*rgb[1] +  0.1140*rgb[2];
}


inline tiny_vec<float,3> gray_2_rgb(const float gray)
{	
	return tiny_vec<float,3>(gray,gray,gray);
}


inline tiny_vec<double,3> gray_2_rgb(const double gray)
{	
	return tiny_vec<double,3>(gray,gray,gray);
}


inline tiny_vec<unsigned char,3> gray_2_rgb(const unsigned char gray)
{	
	return tiny_vec<unsigned char,3>(gray,gray,gray);
}


inline tiny_vec<float,3> uchar_2_float(const tiny_vec<unsigned char,3>& uchar_vec)
{
	return tiny_vec<float,3>(uchar_vec[0]/255.0f,uchar_vec[1]/255.0f,uchar_vec[2]/255.0f);
}


inline tiny_vec<double,3> uchar_2_double(const tiny_vec<unsigned char,3>& uchar_vec)
{
	return tiny_vec<double,3>(uchar_vec[0]/255.0,uchar_vec[1]/255.0,uchar_vec[2]/255.0);
}



inline tiny_vec<unsigned char,3> float_2_uchar(const tiny_vec<float,3>& fvec)
{
	return tiny_vec<unsigned char,3>((unsigned char)(fvec[0]*255.0f),(unsigned char)(fvec[1]*255.0f),(unsigned char)(fvec[2]*255.0f));
}


inline tiny_vec<unsigned char,3> double_2_uchar(const tiny_vec<double,3>& dvec)
{
	return tiny_vec<unsigned char,3>((unsigned char)(dvec[0]*255.0),(unsigned char)(dvec[1]*255.0),(unsigned char)(dvec[2]*255.0));
}






#endif