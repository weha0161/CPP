#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include "../Wrapper/Wrapper.hpp"
#include "../Traits/Traits.h"
#include "../Typelist/Typelist.h"

#ifndef STRING_H
#define STRING_H

namespace String_
{
	//---------------------------------------------------------------------------------------------------Split----------------------------------------------------------------------------------------

	using Delimiter = T::char_<'/'> ;
	
	T::Is_<Delimiter> isDelimiter;
	T::IsNot_<Delimiter> isNotDelimiter;
		
	template<typename Type = Delimiter>
	std::vector<std::string> Split(const std::string str)
	{
		std::vector<std::string> result;		
		using iter = std::string::const_iterator;
		
		iter i = str.begin();
		
		while(i != str.end())
		{
			i = std::find_if(i, str.end(), [](auto c){ return Type::Value != c; });
			
			iter j = std::find_if(i, str.end(), [](auto c){ return Type::Value == c; });
			
			if(i != str.end())
				result.push_back(std::string(i,j));
			
			i = j;
		}
				
		return result;
	};
	
	bool Contains(std::string s, std::string sub)
	{
		return s.find(sub) != std::string::npos; 
	}
	
	template<typename T>
	std::string Remove(const std::string& s)
	{
		std::string result = s;
		result.erase(std::remove(result.begin(), result.end(), T::Value), result.end());
		
		return result;
	}
	
	template<typename T>
	struct From
	{
		std::string operator()(T t) { return ""; }
	};
	
	template<>
	struct From<int>
	{
		std::string operator()(int t) { return std::to_string(t); }
	};
	
	static From<int> FromInt = From<int>();
}

#endif
