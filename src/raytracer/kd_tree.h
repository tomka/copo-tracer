#ifndef KD_TREE_H
#define KD_TREE_H

#include "primitive_group.h"
#include "box.h"


//kd_tree_group
class kd_tree : public primitive_group
{
public:
	

	//abstract class of a node of a kd-tree.
	class kd_node
	{
	public:
		virtual ~kd_node(){}
		virtual bool is_leaf() const=0;
		
	};

	
	//root node of the tree
    kd_node* root;
	// bounding box of the whole tree
    box bounds;
	//The maximum depth of the tree
    unsigned int max_depth;
	//The minimum number of primitives per leaf node.
    unsigned int min_size;
	


	
 
 public:
		

	kd_tree(unsigned int max_depth=30, unsigned int  min_size = 4);
	 
	void add_obj_mesh(const std::string& filename, bool scale=true);

	void add_primitive(primitive* p);
	
	void clear();
	
	void build_tree_spatial_median();
	
	~kd_tree();
	
 
 // kD-tree traversal algorithm TA_rec_B by Vlastimil Havran
 // see Havran's dissertation thesis (Nov 2000), Appendix C

	virtual bool closest_intersection(intersection_info* hit,float min_lambda);
	
	virtual bool any_intersection(ray<float>& r,float min_lambda, float max_lambda);

protected:
	kd_node* build_tree_spatial_median(  std::vector<primitive*> &primitives, box &bounds, unsigned int depth );
	

	
};
#endif