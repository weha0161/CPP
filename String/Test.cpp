#include <iostream>
#include "String_.hpp"
#include "../Traits/Traits.h"
// #include "../AbstractFactory/AbstractFactory.hpp"

int main()
{       

	using a = T::char_<'a'> ;
	
	T::Is_<a> isA;
	T::IsNot_<a> isNA;
	
	std::cout<<"a == a "<<isA('a')<<std::endl;
	std::cout<<"a == a "<<isNA('a')<<std::endl;
	std::cout<<"a == 64 "<<isNA(64)<<std::endl;
	
	const std::string pathToShow{ "/home/markus/Dokumente/cpp/File" };

    auto c = String_::Split(pathToShow);
    
    for(auto d : c)
    	std::cout<<d<<std::endl;
    return 0;
}
