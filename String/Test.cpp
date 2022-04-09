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
	
	const std::string pathToShow{ "/home/markus/Dokumente/cpp/File" };

    auto c = String_::Split<del>(pathToShow);
	auto p = String_::Parser();
	
	auto pi = std::make_shared<String_::ParsedInt>(std::make_shared<std::string>("3"));
	auto pw = std::make_shared<String_::ParsedWord>(std::make_shared<std::string>("TRE"), pi);
	
	auto pwn = std::dynamic_pointer_cast<String_::ParsedInt>(pw->Next());
	
    std::cout<<pwn->Value()<<std::endl;
	/*
	Atom A;
	auto ns = std::string("Name 29 Test");
	auto r = atom->make(ns);
    std::cout<<atom->Value()<<std::endl;
	std::cout<<"a == 65 "<<isNA(64)<<std::endl;
	std::cout<<"a == 65 "<<isNA(64)<<std::endl;
    std::cout<<NumberInstance.Name()<<std::endl;
    std::cout<<NumberInstance.Value()<<std::endl;
    std::cout<<r->Value()<<std::endl;
    std::cout<<r->Name()<<std::endl;*/
    return 0;
}
