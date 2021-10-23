#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include "../Wrapper/Wrapper.hpp"
#include "../Traits/Traits.h"
#include "../Typelist/Typelist.h"

#ifndef STRING_H
#define STRING_H

// //--------------------------------STRING_------------------------------------------------
// class String_
// {
// private:
// 	
// public:
// 	const std::string Value;
// 	const int Size;
// 	String_(std::string s): Value(s),Size(s.length()) {};
// 	operator std::string() const { return Value; }	
// };

namespace String_
{
	//---------------------------------------------------------------------------------------------------Split----------------------------------------------------------------------------------------

	using Delimiter = T::char_<'/'> ;
	
	T::Is_<Delimiter> isDelimiter;
	T::IsNot_<Delimiter> isNotDelimiter;
		
	template<typename Type = Delimiter>
	std::vector<std::string> Split(const std::string lineArg)
	{
		std::vector<std::string> lineValues;
		std::string line = lineArg;
		std::string delimiter = {Type::Value};
		size_t pos = 0;
		std::string token;
		
		while ((pos = line.find(delimiter)) != std::string::npos) {
		
			token = line.substr(0, pos);        
			line.erase(0, pos + delimiter.length());
			lineValues.push_back(token);
		}
				
		return lineValues;
	};
	
	bool Contains(std::string s, std::string sub)
	{
		return s.find(sub) != std::string::npos; 
	}
}

#endif
