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
	
    std::cout<<"Key: "<<e->Value<<std::endl;
   
	
    std::cout<<"END"<<std::endl;

    return 0;
}
