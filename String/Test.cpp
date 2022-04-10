#include <iostream>
#include "String_.hpp"
#include "AtomPassingParser.hpp"
#include "ParsedValues.hpp"
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
	
	const std::string pathToShow{ "../ home/ markus/Dok umente/cpp/Fi\nle23" };

    auto c = String_::Split<del>(pathToShow);
	auto p = String_::Parser();
	
	
	auto pi = std::make_shared<String_::ParsedInt>(std::make_shared<std::string>("3"));
	auto pw = std::make_shared<String_::ParsedWord>(std::make_shared<std::string>("TRE_84684"), pi);
	
	auto pwn = std::dynamic_pointer_cast<String_::ParsedInt>(pw->Next());
	
    std::cout<<pwn->Value()<<std::endl;
	
	p.Parse(pathToShow);
    
    return 0;
}
