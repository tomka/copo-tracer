#include "primitive.h"
#include <limits>






void primitive::set_name(const std::string& s)
{
	name = s;
}
std::string primitive::get_name() const 
{
	return name;
}

primitive::primitive()
{
	pmaterial=NULL;
}
primitive::primitive(material* m)
{
	pmaterial = m;
}

void primitive::set_material(material* m)
{
	pmaterial=m;
}

material* primitive::get_material() const
{
	return pmaterial;
}

std::pair<tiny_vec<float,3>, tiny_vec<float,3> > primitive::calc_bounds()
{	
	tiny_vec<float,3 > a(-(std::numeric_limits<float>::infinity)(),-(std::numeric_limits<float>::infinity)(),-(std::numeric_limits<float>::infinity)());
	tiny_vec<float,3 > b((std::numeric_limits<float>::infinity)(),(std::numeric_limits<float>::infinity)(),(std::numeric_limits<float>::infinity)());
	return std::make_pair(a,b);
};





