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
	

	template<typename Type = Delimiter, typename is = T::Is_<Type>, typename isNot = T::IsNot_<Type>>
	std::vector<std::string> Split(const std::string s)
	{
		using iter = std::string::const_iterator;
		std::vector<std::string> v;
		
		iter i = s.cbegin();
		while (i != s.cend())
		{
			i = find_if(i,s.cend(), isNot());
			iter j = find_if(i,s.cend(), is());
		
			if (i != s.cend())
			{
				v.push_back(std::string(i,j));
			}
			
			i = j;
		}
		
		return v;
	};
	
	bool Contains(std::string s, std::string sub)
	{
		return s.find(sub) != std::string::npos; 
	}
}

#endif
