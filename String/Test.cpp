#include <iostream>
#include "String_.hpp"
#include "Parser.hpp"
#include "../Traits/Traits.h"
// #include "../AbstractFactory/AbstractFactory.hpp"
using namespace String_;

int main()
{       

	using a = T::char_<'a'> ;
	using del = T::char_<'/'> ;
	
	T::Is_<a> isA;
	T::IsNot_<a> isNA;
	
	std::cout<<"a == a "<<isA('a')<<std::endl;
	std::cout<<"a == a "<<isNA('a')<<std::endl;
	std::cout<<"a == 64 "<<isNA(64)<<std::endl;
	
	const std::string pathToShow{ "/home/markus/Dokumente/cpp/File" };

    auto c = String_::Split<del>(pathToShow);
    std::cout<<"AsString: "<<std::endl;
	
	Atom A;
	
    return 0;
}
