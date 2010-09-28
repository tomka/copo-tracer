// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.


#ifndef MAT_H
#define MAT_H


#include "vec.h"
#include "tiny_vec.h"
#include <fstream>

#ifdef _HAS_CPP0X
#include <random>
#endif




template <typename T>
class mat;




template <typename RandomAccessIterator>
struct step_iterator:
	public std::iterator<typename std::random_access_iterator_tag,typename std::iterator_traits<RandomAccessIterator>::value_type>

{
public:
	typedef std::iterator<typename std::random_access_iterator_tag,typename std::iterator_traits<RandomAccessIterator>::value_type> base_type;
	typedef typename base_type::pointer pointer;
	typedef typename base_type::reference reference;
	typedef typename base_type::value_type value_type;
	typedef typename base_type::difference_type difference_type;
private:
	RandomAccessIterator internal_iter;
	int step;


	friend class mat<value_type>;

	step_iterator( RandomAccessIterator begin,int step=1):internal_iter(begin),step(step) 
	{}
	
	
public:
	
	
	step_iterator():internal_iter(NULL)
	{
		step=0;
	}
    
	step_iterator(const step_iterator& other)
	{
		internal_iter=other.internal_iter;
		step=other.step;
	}

    step_iterator& operator=(const step_iterator& other)
	{
		if(*this != other)
		{
			internal_iter = other.internal_iter;
			step = other.step;
		}
		return *this;
	}

	bool operator==(const step_iterator& other) const
	{
		return internal_iter == other.internal_iter;
	}
     
	bool operator!=(const step_iterator& other) const
	{
		return !(*this==other);
	}

	reference operator*()
	{
		return *internal_iter;
	}
      
	reference operator*() const
	{
		return *internal_iter;
	}
	pointer operator->()
	 {
		  return &**this;
	 }
      
	pointer operator->() const
	 {
		  return &**this;
	 }

      step_iterator& operator ++()
	  {
		  
		  internal_iter+=step;
		  return *this;
	  }

      step_iterator operator ++(int)
	  {
		  step_iterator tmp=*this;
	      ++*this;
		  return tmp;

	  }
	  step_iterator& operator --()
	  { 
		  internal_iter-=step;
		  return *this;
	  }
      step_iterator operator --(int)
	  {
		  step_iterator tmp=*this;
	      --*this;
		  return tmp;
	  }

	 
     
	  step_iterator& operator +=(difference_type n)
	  {
		  internal_iter+=n*step;
		  return *this;
		 
	  }

	  step_iterator& operator -=(difference_type n)
	  {
		  internal_iter-=n*step;
		  return *this;
	  }

	  step_iterator operator -(difference_type n) const
	  {
		  step_iterator tmp=*this;
	      tmp-=n;
		  return tmp;
	  }
	  difference_type operator-(const step_iterator& right) const
		{	
		return (internal_iter - right.internal_iter)/step;
		}
	 

	  step_iterator operator +(difference_type n)const
	  {
		  step_iterator tmp=*this;
	      tmp+=n;
		  return tmp;
	  }

	  reference operator[](difference_type offset) const
	  {
		return (*(*this + offset));
	  }

	  bool operator <(const step_iterator& other) const
	  {
		  if(step > 0)
			return internal_iter < other.internal_iter;
		  else
			return internal_iter > other.internal_iter;
	  }
	  
	  bool operator >(const step_iterator& other) const
	  {
		   if(step > 0)
			   return internal_iter > other.internal_iter;
		   else
			   return internal_iter < other.internal_iter;
	  }

	  bool operator <=(const step_iterator& other) const
	  {
		   if(step > 0)
			   return internal_iter <= other.internal_iter;
		   else
			   return internal_iter >= other.internal_iter;
	  }
	  
	  bool operator >=(const step_iterator& other) const
	  {
		  if(step > 0)
			  return internal_iter >= other.internal_iter;
		  else
			  return internal_iter <= other.internal_iter;
	  }  
};

//column major full matrix 
template <typename T>
class mat
{
	T* _data;
	size_t _nrows,_ncols,_size;
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

	typedef iterator col_iterator;
	typedef const col_iterator const_col_iterator;
	typedef std::reverse_iterator<col_iterator> reverse_col_iterator;
	typedef std::reverse_iterator<const_col_iterator> const_reverse_col_iterator;

	typedef step_iterator<iterator> row_iterator;
	typedef const row_iterator const_row_iterator;
	typedef std::reverse_iterator<row_iterator> reverse_row_iterator;
	typedef std::reverse_iterator<const_row_iterator> const_reverse_row_iterator;

	typedef step_iterator<iterator > diag_iterator;
	typedef const diag_iterator const_diag_iterator;
	typedef std::reverse_iterator<diag_iterator> reverse_diag_iterator;
	typedef std::reverse_iterator<const_diag_iterator> const_reverse_diag_iterator;

	typedef step_iterator<iterator > anti_diag_iterator;
	typedef const anti_diag_iterator const_anti_diag_iterator;
	typedef std::reverse_iterator<anti_diag_iterator> reverse_anti_diag_iterator;
	typedef std::reverse_iterator<const_anti_diag_iterator> const_reverse_anti_diag_iterator;



	//default constructor
	mat():_data(NULL),_nrows(0),_ncols(0),_size(0)
	{
	}
	//constructs a matrix with m rows and n columns
	mat(size_t m, size_t n):_nrows(m),_ncols(n),_size(m*n)
	{
		_data = new T[_size];
	}
	//copy constructor
	mat(const mat<T>& m)
	{
		_nrows = m._nrows;
		_ncols = m._ncols;
		_size  = _nrows*_ncols;
		_data = new T[_size];
		std::copy(m.begin(),m.end(),begin());
	}
#ifdef _HAS_CPP0X
	//move constructor
	mat(mat<T> && v):_nrows(0),_ncols(0),_size(0),_data(NULL)
	{
		std::swap(_nrows,v._nrows);
		std::swap(_ncols,v._ncols);
		std::swap(_size,v._size);
		std::swap(_data,v._data);
	}
#endif
	~mat()
	{
		clear();
	}

	void clear()
	{
		if(_data)
		{
			delete [] _data;
			_data=NULL;
			_size=0;
			_nrows=0;
			_ncols=0;
		}
	}

	size_t size() const
	{
		return _size;
	}

	size_t nrows() const
	{
		return _nrows;
	}
	
	size_t w() const
	{
		return _nrows;
	}

	size_t h() const
	{
		return _ncols;
	}

	size_t ncols() const
	{
		return _ncols;
	}

	operator T*()
	{
		return _data;
	}

	operator const T*() const
	{
		return _data;
	}

	mat& operator=(const mat<T>& m)
	{
		if(this == &m)
			return *this;
		resize(m._nrows,m._ncols);
		std::copy(m.begin(),m.end(),begin());
		return *this;

	}

	template <typename S>
	operator mat<S>()
	{
		mat<S> u(_nrows,_ncols);
		typename mat<S>::iterator it = u.begin();
#ifdef _HAS_CPP0X
		std::for_each(begin(),end(),[&it](const T& v){*it++ = (S) v; });
#else
		typename mat<S>::iterator end = u.end();
		iterator vit = begin();
		while(it != end)
			*it++ = (S) *vit++;
#endif
		return u;
	}

#ifdef _HAS_CPP0X
	mat& operator=(mat<T>&& m)
	{
		if(this == &m)
			return *this;
		std::swap(_nrows,m._nrows);
		std::swap(_ncols,m._ncols);
		std::swap(_size,m._size);
		std::swap(_data,m._data);
		return *this;
	}
#endif

	mat& operator=(const T& s)
	{
#ifdef _HAS_CPP0X
		std::for_each(begin(),end(),[&s](T& elem){elem=s;});
#else
		for(int i = 0; i < (int)_size; i++)
			_data[i] = s;
#endif
		return *this;
	}	
	
	mat operator-() const
	{
		
		return  (T)(-1)* (*this);
	}

	mat& operator+=(const T& s)
	{
#ifdef _HAS_CPP0X
		std::for_each(begin(),end(),[&s](T& elem){elem+=s;});
#else
		for(int i = 0; i < _size; i++)
			_data[i] += s;
#endif
		return *this;
	}

	mat operator+(const T& s) const
	{
		mat r = *this;
		r += s;
		return r;
	}

	mat& operator-=(const T& s)
	{
#ifdef _HAS_CPP0X
		std::for_each(begin(),end(),[&s](T& elem){elem-=s;});
#else
		for(int i = 0; i < _size; i++)
			_data[i] -= s;
#endif
		return *this;
	}

	mat operator-(const T& s) const
	{
		mat r = *this;
		r -= s;
		return r;
	}

	mat& operator*=(float s)
	{
#ifdef _HAS_CPP0X
		std::for_each(begin(),end(),[&s](T& elem){elem *= s;});
#else
		for(int i = 0; i < (int)_size; i++)
			_data[i] *= s;
#endif
		return *this;
	}

	mat& operator*=(double s)
	{
#ifdef _HAS_CPP0X
		std::for_each(begin(),end(),[&s](T& elem){elem *= s;});
#else
		for(int i = 0; i < (int)_size; i++)
			_data[i] *= s;
#endif
		return *this;
	}

	mat operator*(float s) const
	{
		mat r = *this;
		r *= s;
		return r;
	}
	mat operator*(double s) const
	{
		mat r = *this;
		r *= s;
		return r;
	}

	mat& operator/=(const T& s)
	{
#ifdef _HAS_CPP0X
		std::for_each(begin(),end(),[&s](T& elem){elem/=s;});
#else
		for(int i = 0; i < _size; i++)
			_data[i] /= s;
#endif
		return *this;
	}
	mat operator/(const T& s) const
	{
		mat r = *this;
		r /= s;
		return r;
	}
	mat& operator+=(const mat& m)
	{
#ifdef _HAS_CPP0X
		mat::const_iterator mit=m.begin();
		std::for_each(begin(),end(),[&mit](T& elem){elem+=*mit++;});
#else
		for(int i=0; i < _size; i++)
			_data[i] = m._data[i];
#endif
		return *this;
	}

	mat operator+(const mat&m) const
	{
		mat r =m;
		r+=*this;
		return r;
	}

	mat operator-(const mat&m) const
	{
		mat r =*this;
		r-=m;
		return r;
	}

	mat& operator-=(const mat& m)
	{
#ifdef _HAS_CPP0X
		mat::const_iterator mit=m.begin();
		std::for_each(begin(),end(),[&mit](T& elem){elem-=*mit++;});
#else
		for(int i = 0; i < _size; i++)
			_data[i] -= m._data[i];
#endif
		return *this;
	}

	void reshape(size_t m, size_t n)
	{
		assert(m*n == _size);
		_nrows = m;
		_ncols = n;
	}
	void resize(size_t m, size_t n)
	{
  		if(_nrows == m && _ncols == n)
			return;
		if(_data)
			delete [] _data;
		_nrows = m;
		_ncols = n;
		_size = m*n;
		if(_size > 0)
			_data = new T[_size];	
	}

	void flipud()
	{
		int i = 0;
		int j = _ncols-1;
		
		while(i < j)
		{
			std::swap_ranges(col_begin(i),col_end(i),col_begin(j));
			
			i++;
			j--;
		}
	}
	void fliplr()
	{
		int i = 0;
		int j = _nrows-1;
		
		while(i < j)
		{
			std::swap_ranges(row_begin(i),row_end(i),row_begin(j));
			
			i++;
			j--;
		}
	}

	void zeros(size_t m, size_t n)
	{
		resize(m,n);
		T zero = 0;
		std::fill(begin(),end(),zero);
	}

	void zeros(size_t m)
	{
		resize(m,m);
		T zero;
		zero= 0;
		std::fill(begin(),end(),zero);
	}

	void ones(size_t m)
	{
		resize(m,m);
		T one;
		one = 1;
		std::fill(begin(),end(),one);
	}

	void ones(size_t m, size_t n)
	{
		resize(m,n);
		T one;
		one = 1;
		std::fill(begin(),end(),one);
	}

	void fill(const T& s)
	{
		std::fill(begin(),end(),s);
	}

	

	T& operator()(size_t i)
	{
		return _data[i];
	}

	T operator()(size_t i) const
	{
		return _data[i];
	}

	T& operator[](size_t i)
	{
		return _data[i];
	}

	T operator[](size_t i) const
	{
		return _data[i];
	}

	T& operator()(size_t i, size_t j)
	{
		assert(i < _nrows && j < _ncols);
		return _data[j*_nrows +i];
	}

	T operator()(size_t i, size_t j) const
	{
		assert(i < _nrows && j < _ncols);
		return _data[j*_nrows + i];
	}


	vec<T> col(size_t i) const
	{
		vec<T> v(_nrows);
		std::copy(col_begin(i),col_end(i),v.begin());
		return v;
	}
	vec<T> row(size_t i) const
	{
		vec<T> v(_ncols);
		std::copy(row_begin(i),row_end(i),v.begin());
		return v;
	}

	void set_row(size_t i,const vec<T>& v)
	{
		std::copy(v.begin(),v.end(),row_begin(i));
	}

	void set_col(size_t i,const vec<T>& v)
	{
		std::copy(v.begin(),v.end(),col_begin(i));
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
		return _data+ _size;
	}
	col_iterator col_begin(int c)
	{
		return col_iterator(_data+(c*_nrows));
	}
	col_iterator col_end(int c)
	{
		return col_iterator(_data+(c+1)*_nrows );
	}
	const_col_iterator col_begin(int c) const
	{
		return const_col_iterator(_data+(c*_nrows));
	}
	const_col_iterator col_end(int c) const
	{
		return const_col_iterator(_data+(c+1)*_nrows );
	}
	reverse_col_iterator col_rbegin(int c)
	{	
		return (reverse_col_iterator(col_end(c)));
	}
	reverse_col_iterator col_rend(int c)
	{
		return (reverse_col_iterator(col_begin(c)));
	}
	const_reverse_col_iterator col_rbegin(int c) const
	{	
		return (const_reverse_col_iterator(col_end(c)));
	}
	const_reverse_col_iterator col_rend(int c) const
	{	
		return (const_reverse_col_iterator(col_begin(c)));
	}
	row_iterator row_begin(int r)
	{
		return row_iterator(_data+r,_nrows);
	}
	row_iterator row_end(int r)
	{
		return row_iterator(_data+(r+_size),_nrows);
	}
	const_row_iterator row_begin(int r) const
	{
		return const_row_iterator(_data+r,_nrows);
	}
	const_row_iterator row_end(int r) const
	{
		return const_row_iterator(_data+(r+_size),_nrows);
	}
	reverse_row_iterator row_rbegin(int r)
	{	
		return (reverse_row_iterator(row_end(r)));
	}
	reverse_row_iterator row_rend(int r)
	{
		return (reverse_row_iterator(row_begin(r)));
	}
	const_reverse_row_iterator row_rbegin(int r) const
	{	
		return (const_reverse_row_iterator(row_end(r)));
	}
	const_reverse_row_iterator row_rend(int r) const
	{	
		return (const_reverse_row_iterator(row_begin(r)));
	}
	diag_iterator diag_begin(int d=0)
	{
		if(d <= 0)
			return diag_iterator(_data-d ,_nrows+1);
		else
			return diag_iterator(_data+d*_nrows,_nrows+1);
		
	}
	diag_iterator diag_end(int d=0)
	{	
		if(d <= 0)
		{
			int n=std::min<int>(_nrows+d,_ncols); 
			return diag_iterator(_data-d+n*(_nrows+1),_nrows+1);
		}
		else
		{
			int n = std::min<int>(_ncols-d,_nrows);
			return diag_iterator(_data+d*_nrows+n*(_nrows+1),_nrows+1) ;
		}
	}
	const_diag_iterator diag_begin(int d=0) const
	{
		if(d <= 0)
			return const_diag_iterator(_data-d ,_nrows+1);
		else	
			return const_diag_iterator(_data+d*_nrows,_nrows+1);
		
	}
	const_diag_iterator diag_end(int d=0) const
	{
		if(d <= 0)
		{
			int n=std::min<int>(_nrows+d,_ncols); 
			return const_diag_iterator(begin()-d+n*(_nrows+1),_nrows+1);
		}
		else
		{
			int n = std::min<int>(_ncols-d,_nrows);
			return const_diag_iterator(begin()+d*_nrows+n*(_nrows+1),_nrows+1) ;
		}
	}
	reverse_diag_iterator diag_rbegin(int d=0)
	{	
		return (reverse_diag_iterator(diag_end(d)));
	}
	reverse_diag_iterator diag_rend(int d=0)
	{
		return (reverse_diag_iterator(diag_begin(d)));
	}
	const_reverse_diag_iterator diag_rbegin(int d=0) const
	{	
		return (const_reverse_diag_iterator(diag_end(d)));
	}
	const_reverse_diag_iterator diag_rend(int d=0) const
	{	
		return (const_reverse_diag_iterator(diag_begin(d)));
	}
	anti_diag_iterator anti_diag_begin(int d=0)
	{
		if(d >= 0)
			return anti_diag_iterator(_data+(_ncols-1-d)*_nrows ,1-_nrows);
		else
			return anti_diag_iterator(_data.begin()+(_ncols-1)*_nrows-d ,1-_nrows);
	}
	anti_diag_iterator anti_diag_end(int d=0)
	{
		if(d >= 0)
		{
			int n=std::min(_ncols-d,_nrows);
			return anti_diag_iterator(_data+(_ncols-1-d)*_nrows +n*(1-_nrows),1-_nrows);
		}
		else
		{
			int n=std::min(_nrows+d,_ncols);
			return anti_diag_iterator(_data+(_ncols-1)*_nrows-d +n*(1-_nrows),1-_nrows);
		}
	}
	const_anti_diag_iterator anti_diag_begin(int d=0) const
	{
		if(d >= 0)
			return const_anti_diag_iterator(_data+(_ncols-1-d)*_nrows ,1-_nrows);
		else
			return const_anti_diag_iterator(_data+(_ncols-1)*_nrows-d ,1-_nrows);
	}
	const_anti_diag_iterator anti_diag_end(int d=0) const
	{
		if(d >= 0)
		{
			int n=std::min(_ncols-d,_nrows);
			return const_anti_diag_iterator(begin()+(_ncols-1-d)*_nrows +n*(1-_nrows),1-_nrows);
		}
		else
		{
			int n=std::min(_nrows+d,_ncols);
			return const_anti_diag_iterator(begin()+(_ncols-1)*_nrows-d +n*(1-_nrows),1-_nrows);
		}
	}
	reverse_anti_diag_iterator anti_diag_rbegin(int d=0)
	{	
		return (reverse_anti_diag_iterator(anti_diag_end(d)));
	}
	reverse_anti_diag_iterator anti_diag_rend(int d=0)
	{
		return (reverse_anti_diag_iterator(anti_diag_begin(d)));
	}
	const_reverse_anti_diag_iterator anti_diag_rbegin(int d=0) const
	{	
		return (const_reverse_anti_diag_iterator(anti_diag_end(d)));
	}
	const_reverse_anti_diag_iterator anti_diag_rend(int d=0) const
	{	
		return (const_reverse_anti_diag_iterator(anti_diag_begin(d)));
	}

	void transpose()
	{
		
			mat m(_ncols,_nrows);
	   
			for(int i = 0; i < (int)_ncols;i++)
				std::copy(col_begin(i),col_end(i),m.row_begin(i));
			reshape(_nrows,_ncols);
			operator=(m);
		
	}


	void abs()
	{
#ifdef _HAS_CPP0X
		std::for_each(begin(), end(),[](T& v){v=std::abs(v);});
#else
		for(int i = 0; i < _size; i++)
			_data[i] = std::abs(_data[i]);
#endif
	}
	/*
	void resample(int new_nrows, int new_ncols)
	{
		if(new_nrows < (int)_nrows)
		{
			if(new_ncols < (int)_ncols)
				downsample(new_nrows,new_ncols);
			else
			{
				downsample(new_nrows,_ncols);
				upsample(new_nrows,new_ncols);
			}
		}
		else
		{
			if(new_ncols > (int)_ncols)
				upsample(new_nrows,new_ncols);
			else
			{
				upsample(new_nrows,_ncols);
				downsample(new_nrows,new_ncols);
			}
		}
	}*/

	

	//void downsample(int new_nrows, int new_ncols); 
	

	void upsample(int new_nrows, int new_ncols);
	
	
	void resample_bilinear(int new_nrows, int new_ncols);
	
	

};



// y+= a*x
template <typename T, typename It1, typename It2>
void saxpy(const T& a, It1 xfirst, It1 xlast,It2 yfirst)
{
	It1 itx = xfirst;
	It2 ity = yfirst;
	

	while(itx != xlast)
		*ity++ += a* (*itx++);
}

//y += A*x
template <typename T, typename It1, typename It2>
void gaxpy(const mat<T>& A, It1 xfirst, It1 xlast,It2 yfirst)
{
	It1 itx = xfirst;
	It2 ity = yfirst;

	for(size_t i = 0; i < A.ncols(); i++)
		saxpy(*itx++,A.col_begin(i),A.col_end(i),yfirst);
		
}


template <typename T>
vec<T> operator*(const mat<T>& m, const vec<T>& v)
{
	assert(m.ncols() == v.size());
	vec<T> r;
	r.zeros(m.nrows());
	
	typename vec<T>::const_iterator it = v.begin();  
	for(size_t i = 0; i < m.ncols(); i++)
		saxpy(*it++,m.col_begin(i), m.col_end(i),r.begin());
	
	return r;
}

template <typename T,int N>
vec<T> operator*(const mat<T>& m, const tiny_vec<T,N>& v)
{
	assert(m.ncols() == N);
	vec<T> r;
	r.zeros(m.nrows());
	
	typename vec<T>::const_iterator it = v.begin();  
	for(size_t i = 0; i < m.ncols(); i++)
		saxpy(*it++,m.col_begin(i), m.col_end(i),r.begin());
	
	return r;
}

template <typename T>
mat<T> operator*(const mat<T>& m1, const mat<T>& m2)
{
	assert(m1.ncols() == m2.nrows());
	mat<T> r;
	r.zeros(m1.nrows(),m2.ncols());
	for(size_t i = 0; i <m2.ncols(); i++)
		gaxpy(m1,m2.col_begin(i),m2.col_end(i),r.col_begin(i));

	return r;
}


template <typename T>
mat<T> transpose(const mat<T>& m)
{
	mat<T> r = m;
	r.transpose();
	return r;
}



template <typename T>
mat<T> operator+(const T& s, const mat<T>& m)
{
	return m+s;
}

template <typename T>
mat<T> operator-(const T& s, const mat<T>& m)
{
	return -m+s;
}


template <typename T>
mat<T> operator*(float s, const mat<T>& m)
{
	return m*s;
}

template <typename T>
mat<T> operator*(double s, const mat<T>& m)
{
	return m*s;
}

template <typename T>
mat<T> operator/(const T& s, const mat<T>& m)
{
	return m*(T)(1.0/s);
}

template <typename T>
void mat<T>::upsample(int new_nrows, int new_ncols) 
{
	assert(new_nrows >= _nrows);
	assert(new_ncols >= _ncols);
	// Upsample in x-direction
	int aIntermedSize = new_nrows*_ncols;
	T* aIntermedData = new T[aIntermedSize];
	if (new_nrows > (int)_nrows) {
	for (int i = 0; i < aIntermedSize; i++)
		aIntermedData[i] = 0;
	float factor = ((float)new_nrows)/_nrows;
	for (int y = 0; y < (int)_ncols; y++) 
	{
		int aFineOffset = y*new_nrows;
		int aCoarseOffset = y*_nrows;
		int i = aCoarseOffset;
		int j = aFineOffset;
		int aLastI = aCoarseOffset+_nrows;
		int aLastJ = aFineOffset+new_nrows;
		float rest = factor;
		float part = 1.0;
		do {
		if (rest > 1.0) {
			aIntermedData[j] += part*_data[i];
			rest -= part;
			part = 1.0;
			j++;
			if (rest <= 0.0) {
			rest = factor;
			i++;
			}
		}
		else {
			aIntermedData[j] += rest*_data[i];
			part = 1.0f-rest;
			rest = factor;
			i++;
		}
		}
		while (i < aLastI && j < aLastJ);
	}
	}
	else {
	T* aTemp = aIntermedData;
	aIntermedData = _data;
	_data = aTemp;
	}
	// Upsample in y-direction
	delete[] _data;
	int aDataSize = new_nrows*new_ncols;
	_data = new T[aDataSize];
	if (new_ncols > (int)_ncols) {
	for (int i = 0; i < aDataSize; i++)
		_data[i] = 0;
	float factor = ((float)new_ncols)/_ncols;
	for (int x = 0; x < new_nrows; x++) {
		int i = x;
		int j = x;
		int aLastI = _ncols*new_nrows;
		int aLastJ = new_ncols*new_nrows;
		float rest = factor;
		float part = 1.0;
		do {
		if (rest > 1.0f) {
			_data[j] += part*aIntermedData[i];
			rest -= part;
			part = 1.0;
			j += new_nrows;
			if (rest <= 0.0f) {
			rest = factor;
			i += new_nrows;
			}
		}
		else {
			_data[j] += rest*aIntermedData[i];
			part = 1.0f-rest;
			rest = factor;
			i += new_nrows;
		}
		}
		while (i < aLastI && j < aLastJ);
	}
	}
	else {
	T* aTemp = _data;
	_data = aIntermedData;
	aIntermedData = aTemp;
	}
	// Adapt size of matrix
	_nrows = new_nrows;
	_ncols = new_ncols;
	_size = _ncols*_nrows;
	delete[] aIntermedData;
}
	
template <typename T>
void mat<T>::resample_bilinear(int new_nrows, int new_ncols)
{
		
		int aNewSize = new_nrows*new_ncols;
		T* aNewData = new T[aNewSize];
		float factorX = (float)(_nrows)/(new_nrows);
		float factorY = (float)(_ncols)/(new_ncols);
		for (int y = 0; y < new_ncols; y++)
		{
			for (int x = 0; x < new_nrows; x++)
			{
				float ax = (x+0.5f)*factorX-0.5f;
				float ay = (y+0.5f)*factorY-0.5f;
				if (ax < 0) ax = 0.0f;
				if (ay < 0) ay = 0.0f;
				int x1 = (int)ax;
				int y1 = (int)ay;
				int x2 = x1+1;
				int y2 = y1+1;
				float alphaX = ax-x1;
				float alphaY = ay-y1;
				if (x1 < 0) x1 = 0;
				if (y1 < 0) y1 = 0;
				if (x2 >= (int)_nrows) x2 = _nrows-1;
				if (y2 >= (int)_ncols) y2 = _ncols-1;
				T a = (1.0f-alphaX)*_data[x1+y1*_nrows]
				            +alphaX*_data[x2+y1*_nrows];
				T b = (1.0f-alphaX)*_data[x1+y2*_nrows]
				            +alphaX*_data[x2+y2*_nrows];
				T val = (1.0f-alphaY)*a+alphaY*b;
				aNewData[x+y*new_nrows]=val;
			}
		}
		delete[] _data;
		_data = aNewData;
		_nrows = new_nrows;
		_ncols = new_ncols;
		_size = _ncols*_nrows;
		  
}

	




///compute inverse of 2x2 matrix
template <typename T>
mat<T> inv_22(const mat<T>& m) 
{
	
	mat<T> im(2,2);
	T t4 = 1.0 / (-m(0,0) * m(1,1) + m(0,1) * m(1,0));
	im(0,0) = -m(1,1) * t4;
	im(1,0) = m(1,0) * t4;
	im(0,1) = m(0,1) * t4;
	im(1,1) = -m(0,0) * t4;
	
	return im;
}

///compute inverse of 3x3 matrix
template <typename T>
mat<T> inv_33(const mat<T>& m) 
{
	
	mat<T> im(3,3);
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
mat<T> inv_44(const mat<T>& m) 
{
	
	mat<T> im(4,4);
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


template <typename T>
mat<T> abs(const mat<T>& m)
{
	mat<T> r = m;
	r.abs();
	return r;
}



///creates a 4x4 translation matrix
template <typename T>
mat<T> translate_44(const T&x, const T &y, const T&z)
{
	mat<T> m(4,4);
	m(0,0)=1; m(0,1)= 0; m(0,2)= 0; m(0,3)= x;
	m(1,0)=0; m(1,1)= 1; m(1,2)= 0; m(1,3)= y;
	m(2,0)=0; m(2,1)= 0; m(2,2)= 1; m(2,3)= z;
	m(3,0)=0; m(3,1)= 0; m(3,2)= 0; m(3,3)= 1;
	return m;
}


///creates a 4x4 scale matrix
template <typename T>
mat<T> scale_44(const T&sx, const T &sy, const T&sz)
{
	mat<T> m(4,4);
	m(0,0)=sx; m(0,1)= 0; m(0,2)= 0; m(0,3)= 0;
	m(1,0)=0; m(1,1)=sy; m(1,2)= 0; m(1,3)= 0;
	m(2,0)=0; m(2,1)= 0; m(2,2)= sz; m(2,3)= 0;
	m(3,0)=0; m(3,1)= 0; m(3,2)= 0; m(3,3)= 1;
	return m;
}




///creates a 4x4 rotation matrix around the x axis, the angle is in degree   
template<typename T>
 mat<T> rotatex_44(const T& angle)
{
	T angler = angle*(T)3.14159/(T)180.0;
	mat<T> m(4,4);
    m(0,0)=1; m(0,1)= 0; m(0,2)= 0; m(0,3)= 0;
	m(1,0)=0; m(1,1)= (T)cos((double)angler); 
	m(1,2)= -(T)sin((double)angler); m(1,3)= 0;
	m(2,0)=0; m(2,1)= (T)sin((double)angler); 
	m(2,2)= (T)cos((double)angler); m(2,3)= 0;
	m(3,0)=0; m(3,1)= 0; m(3,2)= 0; m(3,3)= 1;
    return m;
}

///creates a 4x4 rotation matrix around the y axis, the angle is in degree  
template<typename T>
mat<T> rotatey_44(const T& angle)
{
	T angler=angle*(T)3.14159/(T)180.0;
    mat<T> m(4,4);
    m(0,0)=(T)cos((double)angler); m(0,1)= 0; 
	m(0,2)= (T)sin((double)angler); m(0,3)= 0;
	m(1,0)=0; m(1,1)=1; m(1,2)= 0; m(1,3)= 0;
	m(2,0)=-(T)sin((double)angler); m(2,1)= 0; 
	m(2,2)= (T)cos((double)angler); m(2,3)= 0;
	m(3,0)=0; m(3,1)= 0; m(3,2)= 0; m(3,3)= 1;
    return m;
}

///creates a 4x4 rotation matrix around the z axis, the angle is in degree  
template<typename T>
mat<T> rotatez_44(const T& angle)
{
	T angler=angle*(T)3.14159/(T)180.0;
	mat<T> m(4,4);
    m(0,0)=(T)cos((double)angler); 
	m(0,1)= -(T)sin((double)angler); m(0,2)= 0; m(0,3)= 0;
	m(1,0)=(T)sin((double)angler); 
	m(1,1)= (T)cos((double)angler); m(1,2)= 0; m(1,3)= 0;
	m(2,0)=0; m(2,1)= 0; m(2,2)= 1; m(2,3)= 0;
	m(3,0)=0; m(3,1)= 0; m(3,2)= 0; m(3,3)= 1;
    return m;
}



///creates a perspective transformation matrix in the same way as gluPerspective does
template<typename T>
mat<T> perspective_44(const T& fovy, const T&aspect, const T& znear, const T& zfar)
{
	T fovyr = (T)(fovy*3.14159/180.0);
	T f = (T)(cos(fovyr/2.0f)/sin(fovyr/2.0f));
	mat<T> m(4,4);
	m(0,0)=f/aspect; m(0,1)= 0; m(0,2)= 0; m(0,3)= 0;
	m(1,0)=0; m(1,1)= f; m(1,2)= 0; m(1,3)= 0;
	m(2,0)=0; m(2,1)= 0; m(2,2)= (zfar+znear)/(znear-zfar); 
	m(2,3)= (2*zfar*znear)/(znear-zfar);
	m(3,0)=0; m(3,1)= 0; m(3,2)= -1; m(3,3)= 0;
	return m;
}

template<typename T>
mat<T> frustrum_44(const T& left,const T& right,const T& top,const T& bottom, const T& znear, const T& zfar)
{
	
	T A = (right + left)/(right - left);
	T B = (top + bottom)/(top - bottom);
	T C = -(zfar + znear)/(zfar - znear);
	T D = -(2*zfar*znear)/(zfar - znear);
	T E = 2*znear/(right-left);
	T F = 2*znear/(top-bottom);
	mat<T> m(4,4);
	m(0,0)=E; m(0,1)= 0; m(0,2)= A; m(0,3)= 0;
	m(1,0)=0; m(1,1)= F; m(1,2)= B; m(1,3)= 0;
	m(2,0)=0; m(2,1)= 0; m(2,2)= C; m(2,3)= D;
	m(3,0)=0; m(3,1)= 0; m(3,2)= -1; m(3,3)= 0;
	return m;
}



template <typename T>
mat<T> eye(size_t m, size_t n)
{
	mat<T> r(m,n);
	r=(T)0;
	std::fill(r.diag_begin(),r.diag_end(),(T)1);
	return r;
}

template <typename T>
mat<T> eye(size_t m)
{
	return eye<T>(m,m);
}

template <typename T>
mat<T> ones(size_t m, size_t n)
{
	mat<T> r(m,n);
	r=(T)1;
	return r;
}

template <typename T>
mat<T> ones(size_t m)
{
	return ones<T>(m,m);
}

#ifdef _HAS_CPP0X
template <typename T>
mat<T> rand(size_t m, size_t n)
{
	
	std::mt19937 engine;
	std::uniform_real_distribution<T> distribution;
	auto generator = std::bind(distribution, engine);

	mat<T> mr(m,n);
	std::for_each(mr.begin(),mr.end(),[&](T& f){f=generator();});
	return mr;
}


template <typename T>
mat<T> randn(size_t m, size_t n)
{
		
	std::mt19937 engine;
	std::normal_distribution<T> distribution((T)0,(T)1);
	auto generator = std::bind(distribution, engine);
	mat<T> mr(m,n);
	std::for_each(mr.begin(),mr.end(),[&](T& f){f=generator();});
	return mr;
}
#endif

template <typename T>
mat<T> zeros(size_t m, size_t n)
{
	mat<T> r(m,n);
	r=(T)0;
	return r;
}

template <typename T>
mat<T> zeros(size_t m)
{
	return zeros<T>(m,m);
}

template <typename T>
mat<T> cross_mat(vec<T>& v)
{
	mat<T> C(3,3);
	typename mat<T>::iterator it = C.begin();
	
	*it++ = 0;
	*it++ = v(2);
	*it++ = -v(1);

	*it++ = -v(2);
	*it++ = 0;
	*it++ = v(0);

	*it++ = v(1);
	*it++ = -v(0);
	*it = 0;

	return C;
}


template <typename T>
mat<T> cross_mat(tiny_vec<T,3>& v)
{
	mat<T> C(3,3);
	typename mat<T>::iterator it = C.begin();
	
	*it++ = 0;
	*it++ = v(2);
	*it++ = -v(1);

	*it++ = -v(2);
	*it++ = 0;
	*it++ = v(0);

	*it++ = v(1);
	*it++ = -v(0);
	*it = 0;

	return C;
}



inline bool read_fmat(mat<float>& m,const std::string& filename)
{

	std::ifstream myFile (filename.c_str(), std::ios::binary);
    if (myFile) 
	{
		int _w,_h;
		myFile.read((char*)&_w, sizeof(int));
		myFile.read((char*)&_h, sizeof(int));
		int n=_w*_h;
		if(n == 0)
		{
			myFile.close();
			return false;
		}

		m.resize(_w,_h);

		myFile.read((char*)&(m(0,0)), n*sizeof(float));	
		myFile.close();
		m.transpose();
		return true;   
    }
	return false;
}

inline bool write_fmat(mat<float>& m, const std::string& filename) 
{
	mat<float> im=m;
	im.transpose();
	int w = im.nrows();
	int h = im.ncols();
	int n=w*h;
	if(w == 0||h == 0)
		return false;

	
	std::ofstream myFile (filename.c_str(), std::ios::binary);
    if (myFile) 
	{
		 myFile.write((char*)&w, sizeof(int));
		 myFile.write((char*)&h, sizeof(int));
		 int n=w*h;
		 if(n > 0)
			myFile.write((char*)&(im(0,0)), n*sizeof(float));
		 myFile.close();
		 return true;   
    }
	return false;
   
}

inline bool read_imat(mat<int>& m,const std::string& filename)
{

	std::ifstream myFile( filename.c_str(),  std::ios::binary);
    if (myFile) 
	{
		int _w,_h;
		myFile.read((char*)&_w, sizeof(int));
		myFile.read((char*)&_h, sizeof(int));
		int n=_w*_h;
		if(n == 0)
		{
			myFile.close();
			return false;
		}

		m.resize(_w,_h);

		myFile.read((char*)&(m(0,0)), n*sizeof(int));	
		myFile.close();
		m.transpose();
		return true;   
    }
	return false;
}

inline bool write_imat(mat<int>& m, const std::string& filename) 
{
	mat<int> im=m;
	im.transpose();
	int w = im.nrows();
	int h = im.ncols();
	int n=w*h;
	if(w == 0||h == 0)
		return false;

	
	std::ofstream myFile (filename.c_str(), std::ios::binary);
    if (myFile) 
	{
		 myFile.write((char*)&w, sizeof(int));
		 myFile.write((char*)&h, sizeof(int));
		 int n=w*h;
		 if(n > 0)
			myFile.write((char*)&(im(0,0)), n*sizeof(int));
		 myFile.close();
		 return true;   
    }
	return false;
   
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const mat<T>& m)
{
	for(size_t i = 0; i < m.nrows(); i++)
	{

		if(m.ncols() > 0)
			out << m(i,0);
		for(size_t j = 1; j < m.ncols();j++)
			out << " "<<m(i,j);

		out << std::endl;
	}
	return out;
}


inline std::ostream& operator<<(std::ostream& out, const mat<unsigned char>& m)
{
	for(size_t i = 0; i < m.nrows(); i++)
	{
#ifdef _HAS_CPP0X
		auto it = m.row_begin(i);
		if(it != m.row_end(i))
			out << *it ++;
		std::for_each(it, m.row_end(i),[&out](const unsigned char& elem){ out << " "<<(int)elem;});
#else
		if(m.ncols() > 0)
			out << m(i,0);
		for(int j = 1; j < (int)m.ncols();j++)
			out << " "<<(int)m(i,j);
#endif
		out << std::endl;
	}
	return out;
}

template <typename T>
std::istream& operator>>(std::istream& in,  mat<T>& m)
{
#ifdef _HAS_CPP0X
	std::for_each(m.begin(), m.end(),[&in]( T& elem){ in >> elem;});
#else
	for(int i = 0;i < (int)m.size(); i++)
		in >> m._data[i];
#endif
	m.transpose();
	return in;
}


template <typename T>
mat<T> rgb_2_gray(const mat<tiny_vec<T,3> > & rgb_img)
{
	mat<T> result(rgb_img.w(),rgb_img.h());	
	typename mat<T>::iterator rit=result.begin();
	
	for(typename mat<tiny_vec<T,3 > >::const_iterator it = rgb_img.begin();it != rgb_img.end(); it++)
		*rit++ = rgb_2_gray(*it);

	return result;
}
/*
template <>
inline void mat<tiny_vec<double,3> >::downsample(int new_nrows, int new_ncols) 
	{
		assert(new_nrows <= (int)_nrows);
		assert(new_ncols <= (int)_ncols);
	  // Downsample in x-direction
	  int aIntermedSize = new_nrows*_ncols;
	  tiny_vec<double,3>* aIntermedData = new tiny_vec<double,3>[aIntermedSize];
	  if (new_nrows < (int)_nrows) {
		for (int i = 0; i < aIntermedSize; i++)
		  aIntermedData[i] = 0;
		float factor = ((float)_nrows)/new_nrows;
		for (int y = 0; y < (int)_ncols; y++) 
		{
		  int aFineOffset = y*_nrows;
		  int aCoarseOffset = y*new_nrows;
		  int i = aFineOffset;
		  int j = aCoarseOffset;
		  int aLastI = aFineOffset+_nrows;
		  int aLastJ = aCoarseOffset+new_nrows;
		  float rest = factor;
		  float part = 1.0f;
		  do {
			if (rest > 1.0f) {
			  aIntermedData[j] += part*_data[i];
			  rest -= part;
			  part = 1.0f;
			  i++;
			  if (rest <= 0.0f) {
				rest = factor;
				j++;
			  }
			}
			else {
			  aIntermedData[j] += rest*_data[i];
			  part = 1.0f-rest;
			  rest = factor;
			  j++;
			}
		  }
		  while (i < aLastI && j < aLastJ);
		}
	  }
	  else {
		tiny_vec<double,3>* aTemp = aIntermedData;
		aIntermedData = _data;
		_data = aTemp;
	  }
	  // Downsample in y-direction
	  delete[] _data;
	  int aDataSize = new_nrows*new_ncols;
	  _data = new tiny_vec<double,3>[aDataSize];
	  if (new_ncols < (int)_ncols) {
		for (int i = 0; i < aDataSize; i++)
		  _data[i] = 0;
		float factor = ((float)_ncols)/new_ncols;
		for (int x = 0; x < new_nrows; x++) 
		{
		  int i = x;
		  int j = x;
		  int aLastI = _ncols*new_nrows+x;
		  int aLastJ = new_ncols*new_nrows+x;
		  float rest = factor;
		  float part = 1.0f;
		  do {
			if (rest > 1.0f)
			{
			  _data[j] += part*aIntermedData[i];
			  rest -= part;
			  part = 1.0f;
			  i += new_nrows;
			  if (rest <= 0.0f) 
			  {
				rest = factor;
				j += new_nrows;
			  }
			}
			else 
			{
			  _data[j] += rest*aIntermedData[i];
			  part = 1.0f-rest;
			  rest = factor;
			  j += new_nrows;
			}
		  }
		  while (i < aLastI && j < aLastJ);
		}
	  }
	  else {
		tiny_vec<double,3>* aTemp = _data;
		_data = aIntermedData;
		aIntermedData = aTemp;
	  }
	  // Normalize
	  double aNormalization = ((double)aDataSize)/size();
	  for (int i = 0; i < aDataSize; i++)
		_data[i] *= aNormalization;
	  // Adapt size of matrix
	  _nrows = new_nrows;
	  _ncols = new_ncols;
	  _size = _ncols*_nrows;
	  delete[] aIntermedData;
	}




template <>
inline void mat<tiny_vec<float,3> >::downsample(int new_nrows, int new_ncols) 
	{
		assert(new_nrows <= (int)_nrows);
		assert(new_ncols <= (int)_ncols);
	  // Downsample in x-direction
	  int aIntermedSize = new_nrows*_ncols;
	  tiny_vec<float,3>* aIntermedData = new tiny_vec<float,3>[aIntermedSize];
	  if (new_nrows < (int)_nrows) {
		for (int i = 0; i < aIntermedSize; i++)
		  aIntermedData[i] = 0;
		float factor = ((float)_nrows)/new_nrows;
		for (int y = 0; y < (int)_ncols; y++) 
		{
		  int aFineOffset = y*_nrows;
		  int aCoarseOffset = y*new_nrows;
		  int i = aFineOffset;
		  int j = aCoarseOffset;
		  int aLastI = aFineOffset+_nrows;
		  int aLastJ = aCoarseOffset+new_nrows;
		  float rest = factor;
		  float part = 1.0f;
		  do {
			if (rest > 1.0f) {
			  aIntermedData[j] += part*_data[i];
			  rest -= part;
			  part = 1.0f;
			  i++;
			  if (rest <= 0.0f) {
				rest = factor;
				j++;
			  }
			}
			else {
			  aIntermedData[j] += rest*_data[i];
			  part = 1.0f-rest;
			  rest = factor;
			  j++;
			}
		  }
		  while (i < aLastI && j < aLastJ);
		}
	  }
	  else {
		tiny_vec<float,3>* aTemp = aIntermedData;
		aIntermedData = _data;
		_data = aTemp;
	  }
	  // Downsample in y-direction
	  delete[] _data;
	  int aDataSize = new_nrows*new_ncols;
	  _data = new tiny_vec<float,3>[aDataSize];
	  if (new_ncols < (int)_ncols) {
		for (int i = 0; i < aDataSize; i++)
		  _data[i] = 0;
		float factor = ((float)_ncols)/new_ncols;
		for (int x = 0; x < new_nrows; x++) 
		{
		  int i = x;
		  int j = x;
		  int aLastI = _ncols*new_nrows+x;
		  int aLastJ = new_ncols*new_nrows+x;
		  float rest = factor;
		  float part = 1.0f;
		  do {
			if (rest > 1.0f)
			{
			  _data[j] += part*aIntermedData[i];
			  rest -= part;
			  part = 1.0f;
			  i += new_nrows;
			  if (rest <= 0.0f) 
			  {
				rest = factor;
				j += new_nrows;
			  }
			}
			else 
			{
			  _data[j] += rest*aIntermedData[i];
			  part = 1.0f-rest;
			  rest = factor;
			  j += new_nrows;
			}
		  }
		  while (i < aLastI && j < aLastJ);
		}
	  }
	  else {
		tiny_vec<float,3>* aTemp = _data;
		_data = aIntermedData;
		aIntermedData = aTemp;
	  }
	  // Normalize
	  double aNormalization = ((double)aDataSize)/size();
	  for (int i = 0; i < aDataSize; i++)
		_data[i] *= aNormalization;
	  // Adapt size of matrix
	  _nrows = new_nrows;
	  _ncols = new_ncols;
	  _size = _ncols*_nrows;
	  delete[] aIntermedData;
}


template <>
inline void mat<tiny_vec<float,2> >::downsample(int new_nrows, int new_ncols) 
	{
		assert(new_nrows <= (int)_nrows);
		assert(new_ncols <= (int)_ncols);
	  // Downsample in x-direction
	  int aIntermedSize = new_nrows*_ncols;
	  tiny_vec<float,2>* aIntermedData = new tiny_vec<float,2>[aIntermedSize];
	  if (new_nrows < (int)_nrows) {
		for (int i = 0; i < aIntermedSize; i++)
		  aIntermedData[i] = 0;
		float factor = ((float)_nrows)/new_nrows;
		for (int y = 0; y < (int)_ncols; y++) 
		{
		  int aFineOffset = y*_nrows;
		  int aCoarseOffset = y*new_nrows;
		  int i = aFineOffset;
		  int j = aCoarseOffset;
		  int aLastI = aFineOffset+_nrows;
		  int aLastJ = aCoarseOffset+new_nrows;
		  float rest = factor;
		  float part = 1.0f;
		  do {
			if (rest > 1.0f) {
			  aIntermedData[j] += part*_data[i];
			  rest -= part;
			  part = 1.0f;
			  i++;
			  if (rest <= 0.0f) {
				rest = factor;
				j++;
			  }
			}
			else {
			  aIntermedData[j] += rest*_data[i];
			  part = 1.0f-rest;
			  rest = factor;
			  j++;
			}
		  }
		  while (i < aLastI && j < aLastJ);
		}
	  }
	  else {
		tiny_vec<float,2>* aTemp = aIntermedData;
		aIntermedData = _data;
		_data = aTemp;
	  }
	  // Downsample in y-direction
	  delete[] _data;
	  int aDataSize = new_nrows*new_ncols;
	  _data = new tiny_vec<float,2>[aDataSize];
	  if (new_ncols < (int)_ncols) {
		for (int i = 0; i < aDataSize; i++)
		  _data[i] = 0;
		float factor = ((float)_ncols)/new_ncols;
		for (int x = 0; x < new_nrows; x++) 
		{
		  int i = x;
		  int j = x;
		  int aLastI = _ncols*new_nrows+x;
		  int aLastJ = new_ncols*new_nrows+x;
		  float rest = factor;
		  float part = 1.0f;
		  do {
			if (rest > 1.0f)
			{
			  _data[j] += part*aIntermedData[i];
			  rest -= part;
			  part = 1.0f;
			  i += new_nrows;
			  if (rest <= 0.0f) 
			  {
				rest = factor;
				j += new_nrows;
			  }
			}
			else 
			{
			  _data[j] += rest*aIntermedData[i];
			  part = 1.0f-rest;
			  rest = factor;
			  j += new_nrows;
			}
		  }
		  while (i < aLastI && j < aLastJ);
		}
	  }
	  else {
		tiny_vec<float,2>* aTemp = _data;
		_data = aIntermedData;
		aIntermedData = aTemp;
	  }
	  // Normalize
	  double aNormalization = ((double)aDataSize)/size();
	  for (int i = 0; i < aDataSize; i++)
		_data[i] *= aNormalization;
	  // Adapt size of matrix
	  _nrows = new_nrows;
	  _ncols = new_ncols;
	  _size = _ncols*_nrows;
	  delete[] aIntermedData;
}



template <>
inline void mat<float>::downsample(int new_nrows, int new_ncols) 
	{
		assert(new_nrows <= (int)_nrows);
		assert(new_ncols <= (int)_ncols);
	  // Downsample in x-direction
	  int aIntermedSize = new_nrows*_ncols;
	  float* aIntermedData = new float[aIntermedSize];
	  if (new_nrows < (int)_nrows) {
		for (int i = 0; i < aIntermedSize; i++)
		  aIntermedData[i] = 0;
		float factor = ((float)_nrows)/new_nrows;
		for (int y = 0; y < (int)_ncols; y++) 
		{
		  int aFineOffset = y*_nrows;
		  int aCoarseOffset = y*new_nrows;
		  int i = aFineOffset;
		  int j = aCoarseOffset;
		  int aLastI = aFineOffset+_nrows;
		  int aLastJ = aCoarseOffset+new_nrows;
		  float rest = factor;
		  float part = 1.0f;
		  do {
			if (rest > 1.0f) {
			  aIntermedData[j] += part*_data[i];
			  rest -= part;
			  part = 1.0f;
			  i++;
			  if (rest <= 0.0f) {
				rest = factor;
				j++;
			  }
			}
			else {
			  aIntermedData[j] += rest*_data[i];
			  part = 1.0f-rest;
			  rest = factor;
			  j++;
			}
		  }
		  while (i < aLastI && j < aLastJ);
		}
	  }
	  else {
		float* aTemp = aIntermedData;
		aIntermedData = _data;
		_data = aTemp;
	  }
	  // Downsample in y-direction
	  delete[] _data;
	  int aDataSize = new_nrows*new_ncols;
	  _data = new float[aDataSize];
	  if (new_ncols < (int)_ncols) {
		for (int i = 0; i < aDataSize; i++)
		  _data[i] = 0;
		float factor = ((float)_ncols)/new_ncols;
		for (int x = 0; x < new_nrows; x++) 
		{
		  int i = x;
		  int j = x;
		  int aLastI = _ncols*new_nrows+x;
		  int aLastJ = new_ncols*new_nrows+x;
		  float rest = factor;
		  float part = 1.0f;
		  do {
			if (rest > 1.0f)
			{
			  _data[j] += part*aIntermedData[i];
			  rest -= part;
			  part = 1.0f;
			  i += new_nrows;
			  if (rest <= 0.0f) 
			  {
				rest = factor;
				j += new_nrows;
			  }
			}
			else 
			{
			  _data[j] += rest*aIntermedData[i];
			  part = 1.0f-rest;
			  rest = factor;
			  j += new_nrows;
			}
		  }
		  while (i < aLastI && j < aLastJ);
		}
	  }
	  else {
		float* aTemp = _data;
		_data = aIntermedData;
		aIntermedData = aTemp;
	  }
	  // Normalize
	  float aNormalization = ((float)aDataSize)/size();
	  for (int i = 0; i < aDataSize; i++)
		_data[i] *= aNormalization;
	  // Adapt size of matrix
	  _nrows = new_nrows;
	  _ncols = new_ncols;
	  _size = _ncols*_nrows;
	  delete[] aIntermedData;
	}

	
template <>
inline void mat<double>::downsample(int new_nrows, int new_ncols) 
	{
		assert(new_nrows <= (int)_nrows);
		assert(new_ncols <= (int)_ncols);
	  // Downsample in x-direction
	  int aIntermedSize = new_nrows*_ncols;
	  double* aIntermedData = new double[aIntermedSize];
	  if (new_nrows < (int)_nrows) {
		for (int i = 0; i < aIntermedSize; i++)
		  aIntermedData[i] = 0;
		float factor = ((float)_nrows)/new_nrows;
		for (int y = 0; y < (int)_ncols; y++) 
		{
		  int aFineOffset = y*_nrows;
		  int aCoarseOffset = y*new_nrows;
		  int i = aFineOffset;
		  int j = aCoarseOffset;
		  int aLastI = aFineOffset+_nrows;
		  int aLastJ = aCoarseOffset+new_nrows;
		  float rest = factor;
		  float part = 1.0f;
		  do {
			if (rest > 1.0f) {
			  aIntermedData[j] += part*_data[i];
			  rest -= part;
			  part = 1.0f;
			  i++;
			  if (rest <= 0.0f) {
				rest = factor;
				j++;
			  }
			}
			else {
			  aIntermedData[j] += rest*_data[i];
			  part = 1.0f-rest;
			  rest = factor;
			  j++;
			}
		  }
		  while (i < aLastI && j < aLastJ);
		}
	  }
	  else {
		double* aTemp = aIntermedData;
		aIntermedData = _data;
		_data = aTemp;
	  }
	  // Downsample in y-direction
	  delete[] _data;
	  int aDataSize = new_nrows*new_ncols;
	  _data = new double[aDataSize];
	  if (new_ncols < (int)_ncols) {
		for (int i = 0; i < aDataSize; i++)
		  _data[i] = 0;
		float factor = ((float)_ncols)/new_ncols;
		for (int x = 0; x < new_nrows; x++) 
		{
		  int i = x;
		  int j = x;
		  int aLastI = _ncols*new_nrows+x;
		  int aLastJ = new_ncols*new_nrows+x;
		  float rest = factor;
		  float part = 1.0f;
		  do {
			if (rest > 1.0f)
			{
			  _data[j] += part*aIntermedData[i];
			  rest -= part;
			  part = 1.0f;
			  i += new_nrows;
			  if (rest <= 0.0f) 
			  {
				rest = factor;
				j += new_nrows;
			  }
			}
			else 
			{
			  _data[j] += rest*aIntermedData[i];
			  part = 1.0f-rest;
			  rest = factor;
			  j += new_nrows;
			}
		  }
		  while (i < aLastI && j < aLastJ);
		}
	  }
	  else {
		double* aTemp = _data;
		_data = aIntermedData;
		aIntermedData = aTemp;
	  }
	  // Normalize
	  double aNormalization = ((double)aDataSize)/size();
	  for (int i = 0; i < aDataSize; i++)
		_data[i] *= aNormalization;
	  // Adapt size of matrix
	  _nrows = new_nrows;
	  _ncols = new_ncols;
	  _size = _ncols*_nrows;
	  delete[] aIntermedData;
	}


*/
inline mat<tiny_vec<float,3> > uchar_2_float(const mat<tiny_vec<unsigned char,3> > & rgb_img)
{
	mat<tiny_vec<float,3> > result(rgb_img.w(),rgb_img.h());	
	mat<tiny_vec<float,3> >::iterator rit=result.begin();
	
	for(mat<tiny_vec<unsigned char,3> >::const_iterator it = rgb_img.begin();it != rgb_img.end(); it++)
		*rit++ = uchar_2_float(*it);

	return result;
}



inline mat<tiny_vec<unsigned char,3> > float_2_uchar(const mat<tiny_vec<float,3> > & rgb_img)
{
	mat<tiny_vec<unsigned char,3> > result(rgb_img.w(),rgb_img.h());	
	mat<tiny_vec<unsigned char,3> >::iterator rit=result.begin();
	
	for(mat<tiny_vec<float,3> >::const_iterator it = rgb_img.begin();it != rgb_img.end(); it++)
		*rit++ = float_2_uchar(*it);

	return result;
}

inline mat<unsigned char> float_2_uchar(const mat<float> & img)
{
	mat<unsigned char > result(img.w(),img.h());	
	mat<unsigned char >::iterator rit=result.begin();
	
	for(mat<float>::const_iterator it = img.begin();it != img.end(); it++)
		*rit++ = (unsigned char)(*it*255.0f);

	return result;
}

inline mat<unsigned char> float_2_uchar(const mat<double> & img)
{
	mat<unsigned char > result(img.w(),img.h());	
	mat<unsigned char >::iterator rit=result.begin();
	
	for(mat<double>::const_iterator it = img.begin();it != img.end(); it++)
		*rit++ = (unsigned char)(*it*255.0);

	return result;
}

/*
template <>
inline void mat<tiny_vec<unsigned char,3> >::downsample(int new_nrows, int new_ncols) 
{
	mat<tiny_vec<float,3> > imgf = uchar_2_float(*this);
	imgf.downsample(new_nrows,new_ncols);
	operator=(float_2_uchar(imgf));
}*/









#endif
