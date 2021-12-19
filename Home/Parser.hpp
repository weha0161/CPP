#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Unit/Unit.h"
// #include "../CSV/CSV.hpp"
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/vector.hpp>
#include <map>
#include <chrono>
#include <ctime>
#include <memory>

#ifndef PARSERS_HPP
#define PARSERS_HPP

namespace Parsers
{	
	class JSON
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
			auto vals = String_::Split<JSON_Separator>(input);
				
			return Parse(*vals.begin(), ++vals.begin(),vals.end());
		}
	};
	
	template<typename T, typename O,typename I = std::string>
	struct Parser
	{
		static O Parse(I i) { return O(); }
	};

	struct Date;

	template<>
	struct Parser<Date, std::chrono::system_clock::time_point,std::string>
	{
		static std::chrono::system_clock::time_point Parse(std::string s) 
		{ 
			std::string res;
			for(auto c : s)
				if(isdigit(c))
					res += c;
			
			Logger::Log<Debug>()<<res<<std::endl;
			
			int d = std::stoi(std::string(res.begin(),res.begin()+2));
			int m = std::stoi(std::string(res.begin()+3,res.begin()+4));
			int y = std::stoi(std::string(res.begin()+4,res.begin()+8));
			
			Logger::Log<Debug>()<<d<<"-"<<m<<"-"<<y<<std::endl;
			return CreateTimePoint(y,m,d); 		
		}
	private:
		static std::chrono::system_clock::time_point CreateTimePoint(int y, int m, int d)
		{
			struct std::tm t;
			t.tm_sec = 0;
			t.tm_min = 0;
			t.tm_hour = 0;
			t.tm_mday = d;
			t.tm_mon = m-1;
			t.tm_year = y - 1900;
			
			std::time_t tt = std::mktime(&t);
			
			return std::chrono::system_clock::from_time_t(tt);
		}
	};
}


#endif
