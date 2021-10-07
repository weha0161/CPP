#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <unordered_map>
#include <boost/mpl/vector.hpp>
#include <boost/filesystem.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/placeholders.hpp>
#include <filesystem>
#include "../Logger/Logger.hpp"
#include "../CSV/CSV.hpp"
#include "../Typelist/Typelist.h"
#include "../Visitor/Visitor.hpp"

#ifndef PARSETYPES_HPP
#define PARSETYPES_HPP

namespace fs = std::filesystem;

namespace FS
{

	class CODE
	{
		
		std::string line;
		uint lineNumber;
	public:
		using ParseType = CODE;
		using ParseCont = std::vector<ParseType>;
		
		CODE(uint lineNo, std::string l): lineNumber(lineNo), line(l){};
		
		const uint LineNumber() const { return lineNumber; }
		const std::string& Line() const { return line; }
		
		static ParseCont Parse(std::vector<std::string> content)
		{
			uint ctr = 0;
			auto result = ParseCont();
			for(auto l : content)
				result.push_back(ParseType(++ctr, l));
			
			return result;
		}
		
	};
	
	std::ostream& operator<<(std::ostream& out, const CODE& c)
	{
		return out<<c.LineNumber()<<"\t"<<c.Line();
	}
	
	class SALES_ENTRY
	{
		using Separator = T::char_<','> ;
		
		Key key;
		Entry cause;
		Date date;
		Value value;
	public:
		using ParseType = SALES_ENTRY;
		using ParseCont = std::multimap<std::string,ParseType>;
		
		SALES_ENTRY(std::string k, std::string c, std::string d, std::string v): key(k), cause(c), date(d), value(v) {};
		
		const Key& GetKey() const { return key; }
		const Entry& GetEntry() const { return cause; }
		const Date& GetDate() const { return date; }
		const Value& GetValue() const { return value; }
		
		static ParseCont Parse(std::vector<std::string> content)
		{
			uint ctr = 0;
			auto result = ParseCont();

			for(auto line : content)
			{
				auto values = GetCsvRowValues(line);
				result.insert(std::pair<std::string, ParseType>(values.at(1), ParseType(values.at(1),values.at(2),values.at(3), values.at(0))));
			}

			return result;
		}
		
		static std::vector<std::string> GetCsvRowValues(const std::string& lineArg)
        {
            std::vector<std::string> lineValues;
            std::string line = lineArg;
            std::string delimiter = {Separator::Value};
            size_t pos = 0;
            std::string token;
            
            while ((pos = line.find(delimiter)) != std::string::npos) {
            
                token = line.substr(0, pos);        
                line.erase(0, pos + delimiter.length());
                lineValues.push_back(token);
            }
                    
            return lineValues;
        };
		
	};
	
	std::ostream& operator<<(std::ostream& out, const SALES_ENTRY& s)
	{
		return out<<s.GetKey()<<"\t"<<s.GetEntry()<<"\t"<<s.GetDate()<<"\t"<<s.GetValue()<<"\t";
	}
}


#endif
