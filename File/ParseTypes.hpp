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

	class Code
	{
		
		std::string line;
		uint lineNumber;
	public:
		using ParseType = Code;
		using ParseCont = std::vector<ParseType>;
		
		Code(uint lineNo, std::string l): lineNumber(lineNo), line(l){};
		
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
	
	std::ostream& operator<<(std::ostream& out, const Code& c)
	{
		return out<<c.LineNumber()<<"\t"<<c.Line();
	}
	
	template<typename T, template<typename, typename> class TCont = std::map, template<typename> class Cont = std::vector>
	class TransactionContainer
	{
	public:
		using KeyType = typename T::KeyType;
	private:
		Cont<KeyType> keys;
		TCont<KeyType, Cont<T>> transactions;
	public:
		void Insert(KeyType k, T t)
		{
			if(!this->Contains(k))
			{
				this->keys.push_back(k);
				this->transactions.insert(std::pair<KeyType, Cont<T>>(k,Cont{ t }));
				return;
			}
			
			this->transactions[k].push_back(t);
		}
		
		bool Contains(KeyType k){ return this->transactions.find(k) != this->transactions.end(); }
		const Cont<KeyType>& Keys() { return keys; }
	};
	
	class AccountTransaction
	{
		using Separator = T::char_<','> ;
		
		Key key;
		Entry cause;
		Date date;
		Value value;
	public:
		using ParseType = AccountTransaction;
		using KeyType = Key;
		using ParseCont = TransactionContainer<ParseType>;
		
		AccountTransaction(std::string k, std::string c, std::string d, std::string v): key(k), cause(c), date(d), value(v) {};
		
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
				
				auto date = values.at(0);
				auto key = values.at(1);
				auto cause = values.at(2);
				auto sum = values.at(3);
				
				if(key != "")
					result.Insert(key, ParseType(key,cause,sum, date));
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
	
	std::ostream& operator<<(std::ostream& out, const AccountTransaction& s)
	{
		return out<<s.GetKey()<<"\t"<<s.GetEntry()<<"\t"<<s.GetDate()<<"\t"<<s.GetValue()<<"\t";
	}
}


#endif
