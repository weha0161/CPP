#include <iostream>
#include "CSVElement.hpp"
#include "../Traits/Traits.h"

int main()
{       
    std::cout<<"START"<<std::endl;
	std::string test = "Test";
	using Types = Typelist<Key,Value>;

	CSVElementFactory<Types> a;

	std::cout<<"Create Key"<<std::endl;
	CSVElement* e = a.Create<Key>(test);
	CSVElement* e1 = a.Create<Value>(test);
	
// 	a.Register(1,  &Key::Create);
    std::cout<<"Key: "<<e->Value<<std::endl;
	e->Do();
	e1->Do();
	
	auto p = a.Create(0,"TEST");
    std::cout<<"Key2: "<<p->Value<<std::endl;
   
	
    std::cout<<"END"<<std::endl;

    return 0;
}
