#include <string>
#include <cstring>
#include <iostream>
#include "../Wrapper/Wrapper.h"
#include "../Traits/Traits.h"
// #include "../AbstractFactory/AbstractFactory.hpp"
#include "../Typelist/Typelist.h"

#ifndef STRING_H
#define STRING_H

//--------------------------------STRING_------------------------------------------------
class String_
{
private:
	
public:
	const std::string Value;
	const int Size;
	String_(std::string s): Value(s),Size(s.length()) {};
	operator std::string() const { return Value; }	
};

#endif
