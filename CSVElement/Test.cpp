#include <iostream>
#include <boost/mpl/vector.hpp>
#include "CSV.hpp"
#include "../Traits/Traits.h"

namespace mpl=boost::mpl;

int main()
{       
    std::cout<<"START"<<std::endl;
	std::string test = "Test";
	using Types = mpl::vector<Key,Value>;

	ElementFactory<Types> a;
	
	auto p = a.Create(0,test);
	auto p1 = a.Create(1,test);

	std::cout<<"Pointer Test Key "<<p->Value<<std::endl;
	p->Do();
	std::cout<<"Pointer Test Value "<<p1->Value<<std::endl;
	p1->Do();
	
	std::cout<<"Test Unregister Value "<<std::endl;
	a.Unregister(1);
// 	auto p2 = a.Create(1,test);
// 	std::cout<<"Pointer Test Value "<<p2->Value<<std::endl;
// 	p2->Do();
	
	
    std::cout<<"END"<<std::endl;

    return 0;
}
