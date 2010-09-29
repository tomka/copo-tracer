// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#ifndef UNION_FIND_H
#define UNION_FIND_H


/**
* A union find data structure
*/
class union_find
{
	int* sz;//sizes
	int* id;//linked ids
	int n_sets; //number of sets
	int n_elems; //number of elements

public:
	///N number of elements and number of initial sets 
	union_find(int N)
	{
		n_elems=N;
		n_sets=N;
		id = new int[n_elems];
		sz = new int[n_elems];
		for(int i = 0; i < n_elems;i++)
		{
			id[i] = i;
			sz[i] = 1;
		}

	}

	//copy constructor
	union_find(const union_find& uf)
	{
		n_sets = uf.n_sets;
		n_elems = uf.n_elems;
		
		if(num_elems > 0)
		{
			id = new int[n_elems];
			sz = new int[n_elems];

			for(int i = 0; i < n_elems;i++)
			{
				id[i] = uf.id[i];
				sz[i] = uf.sz[i];
			}
		}
		
	}
	//assignment operator
	union_find& operator=(const union_find& uf)
	{
		if(&uf ==this)
			return *this;

		if(n_elems > 0)
		{
			delete [] id;
			delete [] sz;
		}
		n_sets = uf.n_sets;
		n_elems = uf.n_elems;
		if(num_elems > 0)
		{
			id = new int[n_elems];
			sz = new int[n_elems];
		
			for(int i = 0; i < num_elems;i++)
			{
				id[i] = uf.id[i];
				sz[i] = uf.sz[i];
			}
		}


		return *this;
	}

	///destructor
	~union_find()
	{
		if(n_elems> 0)
		{
			delete[] sz;
			delete[] id;
		}
	}

	///return number of sets (initially number of all elements)
	 int num_sets()
	 {
		 return n_sets;
	 }

	 //return the number of elements in the set containing x
	 int num_elems(int x)
	 {
		int l= find_setid(x);
		return sz[l];
	 }

	 //returns setid of the first set this can be used in combination with get_next_setid to iterate over all sets
	 //return -1 if no set is available
	 int first_setid()
	 {
		 int x = 0;
		 while(x < n_elems && x != id[x])
			 x++;
		 if(x == n_elems)
			 x= -1;
		 return x;
	 }

	 ///returns the next setid of x or -1 if x is the last one
	 int next_setid(int x)
	 {
		
		 x++;
		 while(x <n_elems &&x != id[x])
			 x++;
		 if(x == n_elems)
			 x= -1;
		 return x;
	 }

	 ///return setid of element x
	 ///return -1 if set of x is not available
	int find_setid(int x)
	{
		if(x < 0 || x >= n_elems)
			return -1;

		while(x != id[x])
		{
			id[x] = id[id[x]];
			x=id[x];
		}
		return x;
	}

	

	///unite the sets containing p and q 
	void unite_sets(int p, int q)
	{
		if(p < 0 || p >= n_elems)
			return -1;
		if(q < 0 || q >= n_elems)
			return -1;
		int i = find_setid(p);
        int j = find_setid(q);
        if (i == j) return;
        if   (sz[i] < sz[j]) { id[i] = j; sz[j] += sz[i]; }
        else                 { id[j] = i; sz[i] += sz[j]; }
        n_sets--;
	}

	///check wether p and q are in the same set
	bool same_set(int p, int q)
	{
		return find_setid(p) == find_setid(q);
	}

};

#endif