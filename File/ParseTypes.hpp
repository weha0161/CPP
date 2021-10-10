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
#include "../Quantity/Quantity.h"
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
		const Cont<T>& operator[](KeyType k){ return this->transactions[k]; }
		
		void Display(std::ostream& out)
		{
			for(auto p : this->transactions)
			{
				out<<p.first<<":"<<std::endl;
				for(auto elem : p.second)
				{
					out<<"\t"<<elem<<std::endl;
				}
				
				out<<std::endl;
			}
		}
	};
	
	template<typename Derived>
	class AccountTransaction
	{
		using Separator = T::char_<';'> ;
		
		Key key;
		Entry cause;
		Date date;
		Quantity<Sum> value;
	public:
		using KeyType = Key;
		using ParseCont = TransactionContainer<Derived>;
		
		AccountTransaction(std::string k, std::string c, double v, std::string d) : key(k), cause(c), date(d), value(v) { };
		
		const Key& GetKey() const { return key; }
		const Entry& GetEntry() const { return cause; }
		const Date& GetDate() const { return date; }
		const Quantity<Sum>& GetValue() const { return value; }
		
		static ParseCont Parse(std::vector<std::string> content)
		{
			uint ctr = 0;
			auto result = ParseCont();

			for(auto line : content)
			{
				auto values = GetCsvRowValues(line);
				
				if (values.size() < 4)
					continue;
				
				auto key = values.at(Derived::KeyIdx);
				if(key != "")
				{
					auto date = values.at(Derived::DateIdx);
					auto cause = values.at(Derived::CauseIdx);
					
					auto n = GetNumericValue(values.at(Derived::QuantityIdx));
					auto sum = n != "" ? std::stod(n) : 0.0 ;
				
					result.Insert(key, Derived(key,cause,sum, date));
				}
				
				
			}

			return result;
		}
		
		static std::string GetNumericValue(std::string s)
		{
			std::string result;
			for (unsigned int i = 0; i < s.size(); ++i)
			{
				if(std::isdigit(s.at(i)))
					result += s.at(i);
			}
			
			if(result.size() > 2)
				result.insert(result.size()-2, ".");
			
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
	
	
	template<int N>
	struct Comdirect: public AccountTransaction<Comdirect<N>>
	{
		enum{ Num = N };
		using Type = Comdirect<N>;
		inline static constexpr int KeyIdx = 3;
		inline static constexpr int CauseIdx = 2;
		inline static constexpr int DateIdx = 0;
		inline static constexpr int QuantityIdx = 4;
		
		Comdirect(std::string k, std::string c, double v, std::string d) : AccountTransaction<Comdirect<N>>(k,c,v, d) {};
	};
	
	template<int N>
	struct Raiba: public AccountTransaction<Raiba<N>>
	{
		enum{ Num = N };
		using Type = Raiba<N>;
		inline static constexpr int KeyIdx = 4;
		inline static constexpr int CauseIdx = 9;
		inline static constexpr int DateIdx = 0;
		inline static constexpr int QuantityIdx = 12;
		
		Raiba(std::string k, std::string c, double v, std::string d) : AccountTransaction<Raiba<N>>(k,c,v, d) {};
	};
	
	template<int N = 0>
	struct Custom: public AccountTransaction<Custom<N>>
	{
		enum{ Num = N };
		using Type = Custom<N>;
		inline static constexpr int KeyIdx = 1;
		inline static constexpr int CauseIdx = 2;
		inline static constexpr int DateIdx = 0;
		inline static constexpr int QuantityIdx = 3;
		
		Custom(std::string k, std::string c, double v, std::string d) : AccountTransaction<Custom<N>>(k,c,v, d) {};
	};
	template<typename T>
	std::ostream& operator<<(std::ostream& out, const AccountTransaction<T>& s)
	{
		return out<<std::setw(30)<<std::left<<s.GetKey()<<std::setw(60)<<s.GetEntry()<<std::setw(20)<<std::right<<s.GetDate()<<std::setw(10)<<s.GetValue()<<"\t";
	}
}


#endif
