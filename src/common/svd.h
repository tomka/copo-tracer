#ifndef SVD_H
#define SVD_H

#include <algorithm>
#include <cmath>
#include <vec.h>
#include <mat.h>



//compute singular value decomposition such that M = U * S* V^T 
//the diagonal vector of S is returned as s
//if econ is set to true the economical svd is computed (reduces version of U see matlab documentation)
template <typename T>
void svd(const mat<T>& M, mat<T>& U, vec<T>& s, mat<T>&V, bool econ=false,T eps=0);


//compute singular value decomposition such that M = U * S* V^T
 //S is returned as a full matrix
//if econ is set to true the economical svd  is computed (reduces version of U see matlab documentation)
template <typename T>
void svd(const mat<T>& M, mat<T>& U, mat<T>& S, mat<T>&V,bool econ=false, T eps=0);


//compute singular values only (faster than full svd)
template <typename T>
void svd(const mat<T> M,  vec<T>& s, T eps=0);


//returns condition number of matrix M
template <typename T>
T cond (const mat<T>& M,T eps=0);  



//returns the pseudo inverse of M
template <typename T>
mat<T> pseudo_inv(const mat<T>& M,T eps =0,T tol=0);


//returns the effective rank of matrix M
template <typename T>
int rank (const mat<T>& M,T eps =0,T tol=0);
   


//implementation goes here



//internal helper function
template <class T>
T hypot(const T &a, const T &b)
{
	
	if (a== 0)
		return std::abs(b);
	else
	{
		T c = b/a;
		return std::fabs(a) * sqrt(1 + c*c);
	}
}


//compute singular value decomposition such that M = U * S* V^T 
//the diagonal vector of S is returned as s
//if econ is set to true the economical svd is computed (reduces version of U see matlab documentation)
template <typename T>
void svd(const mat<T>& M, mat<T>& U, vec<T>& s, mat<T>&V, bool econ,T eps)
{
	
	if(eps == 0)
		eps=std::numeric_limits<T>::epsilon();

	 int m = M.nrows();
     int n = M.ncols();
	// assert(m >= n);
	 bool sw=false;
	 mat<T> A;
	 if(m >= n)
		A=M;
	 else
	 {
		A=transpose(M);
		std::swap(n,m);
		econ=false;
		sw=true;
	 }
	
   
	 int nu = (std::min)(m,n);
	 int mm;
		if(econ)
			 mm = nu;
		 else
			 mm = m;
	 s.resize((std::min)(m+1,n)); 
    
	U.zeros(m, mm); 

     V.resize(n,n);
     vec<T> e(n);
     vec<T> work(m);
	
	 int i=0, j=0, k=0;

      // Reduce A to bidiagonal form, storing the diagonal elements
      // in s and the super-diagonal elements in e.

	 int nct = (std::min)(m-1,n);
	 int nrt = (std::max)(0,(std::min)(n-2,m));
	 for (k = 0; k < (std::max)(nct,nrt); k++)
	 {
         if (k < nct) {

            // Compute the transformation for the k-th column and
            // place the k-th diagonal in s(k).
            // Compute 2-norm of k-th column without under/overflow.
            s(k) = (T)0;
            for (i = k; i < m; i++) 
			{
               s(k) = hypot(s(k),A(i,k));
            }
            if (s(k) != 0.0) 
			{
               if (A(k,k) < 0.0) {
                  s(k) = -s(k);
               }
               for (i = k; i < m; i++) {
                  A(i,k) /= s(k);
               }
               A(k,k) += 1.0;
            }
            s(k) = -s(k);
         }
         for (j = k+1; j < n; j++) {
            if ((k < nct) && (s(k) != (T)0.0))  {

            // Apply the transformation.

               T t =(T)0;
               for (i = k; i < m; i++) {
                  t += A(i,k)*A(i,j);
               }
               t = -t/A(k,k);
               for (i = k; i < m; i++) {
                  A(i,j) += t*A(i,k);
               }
            }

            // Place the k-th row of A into e for the
            // subsequent calculation of the row transformation.

            e(j) = A(k,j);
         }
         if  (k < nct)
		 {

            // Place the transformation in U for subsequent back
            // multiplication.

            for (i = k; i < m; i++) {
               U(i,k) = A(i,k);
            }
         }
         if (k < nrt) {

            // Compute the k-th row transformation and place the
            // k-th super-diagonal in e(k).
            // Compute 2-norm without under/overflow.
            e(k) = (T)0;
            for (i = k+1; i < n; i++) {
               e(k) = hypot(e(k),e(i));
            }
            if (e(k) != 0.0) {
               if (e(k+1) < 0.0) {
                  e(k) = -e(k);
               }
               for (i = k+1; i < n; i++) {
                  e(i) /= e(k);
               }
               e(k+1) += (T)1.0;
            }
            e(k) = -e(k);
            if ((k+1 < m) & (e(k) != 0.0)) {

            // Apply the transformation.

               for (i = k+1; i < m; i++) {
                  work(i) = (T)0.0;
               }
               for (j = k+1; j < n; j++) {
                  for (i = k+1; i < m; i++) {
                     work(i) += e(j)*A(i,j);
                  }
               }
               for (j = k+1; j < n; j++) {
                  T t(-e(j)/e(k+1));
                  for (i = k+1; i < m; i++) {
                     A(i,j) += t*work(i);
                  }
               }
            }
             {

            // Place the transformation in V for subsequent
            // back multiplication.

               for (i = k+1; i < n; i++) {
                  V(i,k) = e(i);
               }
            }
         }
      }

      // Set up the final bidiagonal matrix or order p.

      int p = (std::min)(n,m+1);
      if (nct < n) {
         s(nct) = A(nct,nct);
      }
      if (m < p) {
         s(p-1) = 0.0;
      }
      if (nrt+1 < p) {
         e(nrt) = A(nrt,p-1);
      }
      e(p-1) = 0.0;

      // If required, generate U.

      {
		 

         for (j = nct; j < mm; j++) //mod
		 {
            for (i = 0; i < m; i++) 
			{
               U(i,j) = (T)0.0;
            }
            U(j,j) = (T)1.0;
         }

         for (k = nct-1; k >= 0; k--) 
		 {
            if (s(k) != 0.0) 
			{
               for (j = k+1; j < mm; j++) {
                  T t(0.0);
                  for (i = k; i < m; i++) {
                     t += U(i,k)*U(i,j);
                  }
                  t = -t/U(k,k);
                  for (i = k; i < m; i++) {
                     U(i,j) += t*U(i,k);
                  }
               }
               for (i = k; i < m; i++ ) {
                  U(i,k) = -U(i,k);
               }
               U(k,k) = (T)1.0 + U(k,k);
               for (i = 0; i < k-1; i++) {
                  U(i,k) = (T)0.0;
               }
            } else {
               for (i = 0; i < m; i++) {
                  U(i,k) = (T)0.0;
               }
               U(k,k) = (T)1.0;
            }
         }
      }

      // If required, generate V.

      {
         for (k = n-1; k >= 0; k--) {
            if ((k < nrt) & (e(k) != 0.0)) {
				
				int nu2;
				if(econ)
					nu2=nu;
				else
					nu2= n;
				
               for (j = k+1; j < nu2; j++) {
                  T t(0.0);
                  for (i = k+1; i < n; i++) {
                     t += V(i,k)*V(i,j);
                  }
                  t = -t/V(k+1,k);
                  for (i = k+1; i < n; i++) {
                     V(i,j) += t*V(i,k);
                  }
               }
            }
            for (i = 0; i < n; i++) {
               V(i,k) = 0.0;
            }
            V(k,k) = 1.0;
         }
      }

      // Main iteration loop for the singular values.

      int pp = p-1;
      int iter = 0;
	 

      while (p > 0) {
         int k=0;
		 int kase=0;

         // Here is where a test for too many iterations would go.

         // This section of the program inspects for
         // negligible elements in the s and e arrays.  On
         // completion the variables kase and k are set as follows.

         // kase = 1     if s(p) and e[k-1] are negligible and k<p
         // kase = 2     if s(k) is negligible and k<p
         // kase = 3     if e[k-1] is negligible, k<p, and
         //              s(k), ..., s(p) are not negligible (qr step).
         // kase = 4     if e(p-1) is negligible (convergence).

         for (k = p-2; k >= -1; k--) {
            if (k == -1) {
               break;
            }
            if (abs(e(k)) <= eps*(abs(s(k)) + abs(s(k+1)))) {
               e(k) = 0.0;
               break;
            }
         }
         if (k == p-2) {
            kase = 4;
         } else {
            int ks;
            for (ks = p-1; ks >= k; ks--) {
               if (ks == k) {
                  break;
               }
               T t( (ks != p ? abs(e(ks)) : (T)0.) + 
                          (ks != k+1 ? abs(e(ks-1)) : (T)0.));
               if (abs(s(ks)) <= eps*t)  {
                  s(ks) = 0.0;
                  break;
               }
            }
            if (ks == k) {
               kase = 3;
            } else if (ks == p-1) {
               kase = 1;
            } else {
               kase = 2;
               k = ks;
            }
         }
         k++;

         // Perform the task indicated by kase.

         switch (kase) {

            // Deflate negligible s(p).

            case 1: {
               T f(e(p-2));
               e(p-2) = 0.0;
               for (j = p-2; j >= k; j--) {
                  T t( hypot(s(j),f));
                  T cs(s(j)/t);
                  T sn(f/t);
                  s(j) = t;
                  if (j != k) {
                     f = -sn*e(j-1);
                     e(j-1) = cs*e(j-1);
                  }
                  {
                     for (i = 0; i < n; i++) {
                        t = cs*V(i,j) + sn*V(i,p-1);
                        V(i,p-1) = -sn*V(i,j) + cs*V(i,p-1);
                        V(i,j) = t;
                     }
                  }
               }
            }
            break;

            // Split at negligible s(k).

            case 2: {
               T f(e(k-1));
               e(k-1) = 0.0;
               for (j = k; j < p; j++) {
                  T t(hypot(s(j),f));
                  T cs( s(j)/t);
                  T sn(f/t);
                  s(j) = t;
                  f = -sn*e(j);
                  e(j) = cs*e(j);
                  {
                     for (i = 0; i < m; i++) {
                        t = cs*U(i,j) + sn*U(i,k-1);
                        U(i,k-1) = -sn*U(i,j) + cs*U(i,k-1);
                        U(i,j) = t;
                     }
                  }
               }
            }
            break;

            // Perform one qr step.

            case 3: {

               // Calculate the shift.
   
               T scale = (std::max)((std::max)((std::max)((std::max)(
				   std::abs(s(p-1)),std::abs(s(p-2))),std::abs(e(p-2))), 
				   std::abs(s(k))),std::abs(e(k)));
               T sp = s(p-1)/scale;
               T spm1 = s(p-2)/scale;
               T epm1 = e(p-2)/scale;
               T sk = s(k)/scale;
               T ek = e(k)/scale;
               T b = ((spm1 + sp)*(spm1 - sp) + epm1*epm1)/(T)2.0;
               T c = (sp*epm1)*(sp*epm1);
               T shift = (T)0.0;
               if ((b != (T)0.0) || (c != (T)0.0)) {
                  shift = sqrt(b*b + c);
                  if (b < 0.0) {
                     shift = -shift;
                  }
                  shift = c/(b + shift);
               }
               T f = (sk + sp)*(sk - sp) + shift;
               T g = sk*ek;
   
               // Chase zeros.
   
               for (j = k; j < p-1; j++) {
                  T t = hypot(f,g);
                  T cs = f/t;
                  T sn = g/t;
                  if (j != k) {
                     e(j-1) = t;
                  }
                  f = cs*s(j) + sn*e(j);
                  e(j) = cs*e(j) - sn*s(j);
                  g = sn*s(j+1);
                  s(j+1) = cs*s(j+1);
                   {
                     for (i = 0; i < n; i++) {
                        t = cs*V(i,j) + sn*V(i,j+1);
                        V(i,j+1) = -sn*V(i,j) + cs*V(i,j+1);
                        V(i,j) = t;
                     }
                  }
                  t = hypot(f,g);
                  cs = f/t;
                  sn = g/t;
                  s(j) = t;
                  f = cs*e(j) + sn*s(j+1);
                  s(j+1) = -sn*e(j) + cs*s(j+1);
                  g = sn*e(j+1);
                  e(j+1) = cs*e(j+1);
                  if ( (j < m-1)) {
                     for (i = 0; i < m; i++) {
                        t = cs*U(i,j) + sn*U(i,j+1);
                        U(i,j+1) = -sn*U(i,j) + cs*U(i,j+1);
                        U(i,j) = t;
                     }
                  }
               }
               e(p-2) = f;
               iter = iter + 1;
            }
            break;

            // Convergence.

            case 4: {

               // Make the singular values positive.
   
               if (s(k) <= (T)0.0) {
                  s(k) = (s(k) < (T)0.0 ? -s(k) : (T)0.0);
                   {
                     for (i = 0; i <= pp; i++) {
                        V(i,k) = -V(i,k);
                     }
                  }
               }
   
               // Order the singular values.
   
               while (k < pp) {
                  if (s(k) >= s(k+1)) {
                     break;
                  }
                  T t = s(k);
                  s(k) = s(k+1);
                  s(k+1) = t;
                  if (k < n-1) {
                     for (i = 0; i < n; i++) {
                        t = V(i,k+1); V(i,k+1) = V(i,k); V(i,k) = t;
                     }
                  }
                  if (k < m-1) {
                     for (i = 0; i < m; i++) {
                        t = U(i,k+1); U(i,k+1) = U(i,k); U(i,k) = t;
                     }
                  }
                  k++;
               }
               iter = 0;
               p--;
            }
            break;
         }
	  }
	  //resize U
	  int minm;
	  if(econ)
		minm = (std::min)(m+1,n);
	  else
		minm = m;

	  
	  if(sw)
	  {
		mat<T> temp = U;
		U = V;
		V=temp;
	  }
 }

//compute singular value decomposition such that M = U * S* V^T
 //S is returned as a full matrix
//if econ is set to true the economical svd  is computed (reduces version of U see matlab documentation)
template <typename T>
void svd(const mat<T>& M, mat<T>& U, mat<T>& S, mat<T>&V,bool econ, T eps)
{
	vec<T> s;
	svd(M,U,s,V,econ,eps);
	size_t n  =M.ncols();
	size_t m = M.nrows();
	if(econ && m>=n)
		S.zeros(n,n);
	else
		S.zeros(m,n);
	
	for (size_t i = 0; i < s.size(); i++)   
		 S(i,i) = s(i);

}

//compute singular values only
template <typename T>
void svd(const mat<T> M,  vec<T>& s, T eps)
{
	if(eps == 0)
		eps=std::numeric_limits<T>::epsilon();

	mat<T> U;
	mat<T> V;
	int m = M.nrows();
    int n = M.ncols();

	 mat<T> A;
	 if(m >= n)
		A=M;
	 else
	 {
		A=transpose(M);
		std::swap(n,m);
	 }


	 int nu = (std::min)(m,n);
	 s.resize((std::min)(m+1,n)); 
      
	 U.zeros(m, nu);
     V.resize(n,n);
     vec<T> e(n);
     vec<T> work(m);
	
   
	 int i=0, j=0, k=0;

      // Reduce A to bidiagonal form, storing the diagonal elements
      // in s and the super-diagonal elements in e.

	 int nct = (std::min)(m-1,n);
	 int nrt = (std::max)(0,(std::min)(n-2,m));
	 for (k = 0; k < (std::max)(nct,nrt); k++)
	 {
         if (k < nct) {

            // Compute the transformation for the k-th column and
            // place the k-th diagonal in s(k).
            // Compute 2-norm of k-th column without under/overflow.
            s(k) = (T)0;
            for (i = k; i < m; i++) 
			{
               s(k) = hypot(s(k),A(i,k));
            }
            if (s(k) != 0.0) {
               if (A(k,k) < 0.0) {
                  s(k) = -s(k);
               }
               for (i = k; i < m; i++) {
                  A(i,k) /= s(k);
               }
               A(k,k) += 1.0;
            }
            s(k) = -s(k);
         }
         for (j = k+1; j < n; j++) {
            if ((k < nct) && (s(k) != 0.0))  {

            // Apply the transformation.

               T t(0.0);
               for (i = k; i < m; i++) {
                  t += A(i,k)*A(i,j);
               }
               t = -t/A(k,k);
               for (i = k; i < m; i++) {
                  A(i,j) += t*A(i,k);
               }
            }

            // Place the k-th row of A into e for the
            // subsequent calculation of the row transformation.

            e(j) = A(k,j);
         }
        
         if (k < nrt) {

            // Compute the k-th row transformation and place the
            // k-th super-diagonal in e(k).
            // Compute 2-norm without under/overflow.
            e(k) = 0;
            for (i = k+1; i < n; i++) {
               e(k) = hypot(e(k),e(i));
            }
            if (e(k) != 0.0) {
               if (e(k+1) < 0.0) {
                  e(k) = -e(k);
               }
               for (i = k+1; i < n; i++) {
                  e(i) /= e(k);
               }
               e(k+1) += 1.0;
            }
            e(k) = -e(k);
            if ((k+1 < m) & (e(k) != 0.0)) {

            // Apply the transformation.

               for (i = k+1; i < m; i++) {
                  work(i) = 0.0;
               }
               for (j = k+1; j < n; j++) {
                  for (i = k+1; i < m; i++) {
                     work(i) += e(j)*A(i,j);
                  }
               }
               for (j = k+1; j < n; j++) {
                  T t(-e(j)/e(k+1));
                  for (i = k+1; i < m; i++) {
                     A(i,j) += t*work(i);
                  }
               }
            }
           
         }
      }

      // Set up the final bidiagonal matrix or order p.

      int p = (std::min)(n,m+1);
      if (nct < n) {
         s(nct) = A(nct,nct);
      }
      if (m < p) {
         s(p-1) = (T)0.0;
      }
      if (nrt+1 < p) {
         e(nrt) = A(nrt,p-1);
      }
      e(p-1) = 0.0;



 

      // Main iteration loop for the singular values.

      int pp = p-1;
      int iter = 0;
	  while (p > 0) {
         int k=0;
		 int kase=0;

         // Here is where a test for too many iterations would go.

         // This section of the program inspects for
         // negligible elements in the s and e arrays.  On
         // completion the variables kase and k are set as follows.

         // kase = 1     if s(p) and e[k-1] are negligible and k<p
         // kase = 2     if s(k) is negligible and k<p
         // kase = 3     if e[k-1] is negligible, k<p, and
         //              s(k), ..., s(p) are not negligible (qr step).
         // kase = 4     if e(p-1) is negligible (convergence).

         for (k = p-2; k >= -1; k--) {
            if (k == -1) {
               break;
            }
            if (abs(e(k)) <= eps*(abs(s(k)) + abs(s(k+1)))) {
               e(k) = (T)0.0;
               break;
            }
         }
         if (k == p-2) {
            kase = 4;
         } else {
            int ks;
            for (ks = p-1; ks >= k; ks--) {
               if (ks == k) {
                  break;
               }
               T t( (ks != p ? abs(e(ks)) : (T)0.) + 
                          (ks != k+1 ? abs(e(ks-1)) : (T)0.));
               if (abs(s(ks)) <= eps*t)  {
                  s(ks) = 0.0;
                  break;
               }
            }
            if (ks == k) {
               kase = 3;
            } else if (ks == p-1) {
               kase = 1;
            } else {
               kase = 2;
               k = ks;
            }
         }
         k++;

         // Perform the task indicated by kase.

         switch (kase) {

            // Deflate negligible s(p).

            case 1: {
               T f(e(p-2));
               e(p-2) = (T)0.0;
               for (j = p-2; j >= k; j--) {
                  T t( hypot(s(j),f));
                  T cs(s(j)/t);
                  T sn(f/t);
                  s(j) = t;
                  if (j != k) {
                     f = -sn*e(j-1);
                     e(j-1) = cs*e(j-1);
                  }
                 
               }
            }
            break;

            // Split at negligible s(k).

            case 2: {
               T f(e(k-1));
               e(k-1) = 0.0;
               for (j = k; j < p; j++) {
                  T t(hypot(s(j),f));
                  T cs( s(j)/t);
                  T sn(f/t);
                  s(j) = t;
                  f = -sn*e(j);
                  e(j) = cs*e(j);
                 
               }
            }
            break;

            // Perform one qr step.

            case 3: {

               // Calculate the shift.
   
               T scale = (std::max)((std::max)((std::max)((std::max)(
				   std::abs(s(p-1)),std::abs(s(p-2))),std::abs(e(p-2))), 
				   std::abs(s(k))),std::abs(e(k)));
               T sp = s(p-1)/scale;
               T spm1 = s(p-2)/scale;
               T epm1 = e(p-2)/scale;
               T sk = s(k)/scale;
               T ek = e(k)/scale;
               T b = ((spm1 + sp)*(spm1 - sp) + epm1*epm1)/(T)2.0;
               T c = (sp*epm1)*(sp*epm1);
               T shift = 0.0;
               if ((b != 0.0) || (c != 0.0)) {
                  shift = sqrt(b*b + c);
                  if (b < 0.0) {
                     shift = -shift;
                  }
                  shift = c/(b + shift);
               }
               T f = (sk + sp)*(sk - sp) + shift;
               T g = sk*ek;
   
               // Chase zeros.
   
               for (j = k; j < p-1; j++) {
                  T t = hypot(f,g);
                  T cs = f/t;
                  T sn = g/t;
                  if (j != k) {
                     e(j-1) = t;
                  }
                  f = cs*s(j) + sn*e(j);
                  e(j) = cs*e(j) - sn*s(j);
                  g = sn*s(j+1);
                  s(j+1) = cs*s(j+1);
                 
                  t = hypot(f,g);
                  cs = f/t;
                  sn = g/t;
                  s(j) = t;
                  f = cs*e(j) + sn*s(j+1);
                  s(j+1) = -sn*e(j) + cs*s(j+1);
                  g = sn*e(j+1);
                  e(j+1) = cs*e(j+1);
                 
               }
               e(p-2) = f;
               iter = iter + 1;
            }
            break;

            // Convergence.

            case 4: {

               // Make the singular values positive.
   
               if (s(k) <= (T)0.0) {
                  s(k) = (s(k) < (T)0.0 ? -s(k) : (T)0.0);
                  
               }
   
               // Order the singular values.
   
               while (k < pp) {
                  if (s(k) >= s(k+1)) {
                     break;
                  }
                  T t = s(k);
                  s(k) = s(k+1);
                  s(k+1) = t;
                  
                  k++;
               }
               iter = 0;
               p--;
            }
            break;
         }
	  }

	   
}


//returns condition number of matrix M
template <typename T>
T cond (const mat<T>& M,T eps) 
{
	vec<T> s;
	svd(M,s,eps);
	int m = M.nrows();
    int n = M.ncols(); 
	return s(0)/s((std::min)(m,n)-1);
}


//returns the pseudoinverse of M
template <typename T>
mat<T> pseudo_inv(const mat<T>& M,T eps ,T tol) 
{
	if(eps == 0)
		eps = std::numeric_limits<T>::epsilon();
	int m = M.nrows();
    int n = M.ncols();
	
	mat<T> U,S,V;
	svd(M,U,S,V,true,eps);

	if(tol == 0)
	   tol = (std::max)(m,n)*S(0,0)*eps;

	int w = (std::min)(S.ncols(),S.nrows());

	for(int i = 0; i < w;i++)
		S(i,i) = S(i,i) > tol ?  (T)1.0/S(i,i):(T)0;
		
	return V*transpose(S)*transpose(U);

		
}


//returns the effective rank of a matrix
template <typename T>
int rank (const mat<T>& M,T eps ,T tol) 
{
	if(eps == 0)
		eps = std::numeric_limits<T>::epsilon();
	int m = M.nrows();
    int n = M.ncols();
	vec<T> s;
	svd(M,s,eps);

	if(tol == 0)
	   tol = (std::max)(m,n)*s(0)*eps;

	
    
	int r = 0;
    for (size_t i = 0; i < s.size(); i++) 
	{
         if (s(i) > tol) {
            r++;
         }
    }
    return r;
}


/* only for rank(A) = n-1 (m >= n-1 sigma_n = 0
template <typename T>
vec<T> solve_homog(const mat<T>& A)
{
	mat<T> U,V;
	vec<T> s;
	
	
	svd(A,U,s,V);
	
	return  V.col(V.ncols()-1);
	
}*/
#endif