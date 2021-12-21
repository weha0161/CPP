#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <unordered_map>
#include "../Logger/Logger.hpp"
#include "../CSV/CSV.hpp"
#include "../Quantity/Quantity.h"
#include "../Typelist/Typelist.h"
#include "../Visitor/Visitor.hpp"
#include "../String/String_.hpp"
/*
bool Is(char c){ return c == '/'; }
bool Non(char c) { return !Is(c);}*/

using TextSeparator = T::char_<':'> ;
using BlankSeparator = T::char_<' '> ;

template<typename T>
std::vector<std::string> extract(const std::string s)
{
	auto vals = String_::Split<T>(s);
						
// 	auto it = std::find_if (vals.begin(), vals.end(), [](auto s){return String_::Contains(s,T::Identifier); } );
// 	
// 	if(it == vals.end())
// 		return "";
// 				
// 	return *(++it);

	return vals;
};

class JSONParser
{
	using Type = std::string;
	using ResultType = std::map<Type, Type>;
	using JSON_Separator = T::char_<':'> ;
	using BlankSeparator = T::char_<' '> ;
	ResultType resultMap;

public:	
	template<typename Iterator>
	ResultType Parse(Type key, Iterator first, Iterator end)
	{
		if(first == end)
			return this->resultMap;
		
		auto vals = String_::Split<BlankSeparator>(*first);
		auto value = Type();
				
		for(int i = 0; i < vals.size()-1; ++i)
			value += vals.at(i);
		
		this->resultMap[key] = value;
		this->resultMap.insert({key, value});			
		key = vals.at(vals.size()-1);

		return Parse(key, ++first,end);
	}
	
	ResultType Parse(Type input)
	{
		auto vals = String_::Split<TextSeparator>(input);
			
		return Parse(*vals.begin(), ++vals.begin(),vals.end());
	}
};

int main()
{
    const std::string pathToShow{ "Empfänger: Re-In Retail InternationalKto/IBAN: DE81760100850901210858 BLZ/BIC: PBNKDEFF  Buchungstext: 181064499 433455045 Ref. CS5C16WW4BF542EF/1" };

	JSONParser json;
	
	auto result = json.Parse(pathToShow);
//     auto c = extract<TextSeparator>(pathToShow);
//     
//     for(auto d : c)
// 	{
// // 		std::cout<<"1: "<<d<<std::endl;
// 		auto b = extract<BlankSeparator>(d);
// 		
// 		if(b.end() != std::npos)
// 			std::cout<<d<<"\t"<<*b.end()<<std::endl;
// 	}
// 	
// 	for(auto i = c.begin(); i != c.end(); ++i)
// 		result[*i] = *(i+1);
	
	for (auto it = result.begin(); it != result.end(); it++)
	{
		std::cout << it->first    // string (key)
				<< ':'
				<< it->second   // string's value 
				<< std::endl;
	}

	
    return 0;
};
