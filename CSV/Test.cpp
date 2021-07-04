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
	
	auto pk = a.Create("Key",test);
	auto pv = a.Create("Value",test);

	std::cout<<"Pointer Test Key "<<pk->Value<<std::endl;
	pk->Do();
	std::cout<<"Pointer Test Value "<<pv->Value<<std::endl;
	pv->Do();
	
	std::cout<<"Test Unregister Value "<<std::endl;
	
	
    std::cout<<"END"<<std::endl;

    return 0;
}
