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
#include "../String/String_.hpp"

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
				
				typename T::QunatityType total(0.0);
				for(auto elem : p.second)
				{
					total = total + elem.GetQuantity();
					out<<"\t"<<elem<<std::endl;
				}
				
				out<<"Total: "<<total<<std::endl;
				
				out<<std::endl;
			}
		}
	};
	
	template<typename Derived>
	class AccountTransaction
	{
		using Type = AccountTransaction<Derived> ;
		
		Key key;
		Entry cause;
		Date date;
		IBAN iban;
		BIC bic;
		Quantity<Sum> value;

	protected:
		using CSVSeparator = T::char_<';'> ;
// 		using TextSeparator = T::char_<' '> ;
	public:
		using KeyType = Key;
		using ParseCont = TransactionContainer<Derived>;
		using QunatityType = Quantity<Sum>;
		
		AccountTransaction(std::string k, std::string c, double v, std::string d, std::string i = "IBAN", std::string b = "BIC") : key(k), cause(c), date(d), value(v), iban(i), bic(b) { };
		
		const Key& GetKey() const { return key; }
		const Entry& GetEntry() const { return cause; }
		const Date& GetDate() const { return date; }
		const IBAN& GetIBAN() const { return iban; }
		const BIC& GetBIC() const { return bic; }
		const Quantity<Sum>& GetQuantity() const { return value; }
		static constexpr unsigned int Indices[4] = {Derived::KeyIdx, Derived::DateIdx, Derived::CauseIdx, Derived::QuantityIdx};
		static const unsigned int MaxIdx = *std::max_element(Indices,Indices+4);
		
		static void Parse(std::vector<std::string> content)
		{
			uint ctr = 0;

			for(auto line : content)
			{
				auto values = String_::Split<CSVSeparator>(line);
				
				if (values.size() < MaxIdx)
					continue;
				
				auto keyLine = values.at(Derived::KeyIdx);
				if(keyLine != "")
				{
					auto key = Derived::ExtractKey(keyLine);
					auto date = values.at(Derived::DateIdx);
					auto cause = values.at(Derived::CauseIdx);
					
					auto n = GetNumericValue(values.at(Derived::QuantityIdx));
					auto sum = n != "" ? std::stod(n) : 0.0 ;
				
					auto iban = Derived::template Extract<IBAN>(key);
					auto bic =Derived::template Extract<BIC>(key);

					Logger::Log()<<"KEY: "<<key<<" LINE"<<keyLine<<std::endl;
					Derived::Transactions.Insert(key, Derived(key,cause,sum, date, iban, bic));
				}
				
				
			}

			return;
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
				
		template<typename Cont>
		static void AttachTo(Cont& cont)
		{
			cont.insert(std::make_pair(Derived::Filename,  &Type::Parse));
		}	
	};
	
	
	template<unsigned int N>
	struct Comdirect: public AccountTransaction<Comdirect<N>>
	{
		enum{ Num = N };
		using Type = Comdirect<N>;
		inline static const std::string Name = "Comdirect";
		inline static const std::string Filename = "Umsaetze_1026947527.csv";
		inline static constexpr unsigned int KeyIdx = 3;
		inline static constexpr unsigned int CauseIdx = 2;
		inline static constexpr unsigned int DateIdx = 0;
		inline static constexpr unsigned int QuantityIdx = 4;
		
		inline static AccountTransaction<Type>::ParseCont Transactions = typename AccountTransaction<Type>::ParseCont();
		Comdirect(std::string k, std::string c, double v, std::string d, std::string i = "IBAN", std::string b = "BIC") : AccountTransaction<Comdirect<N>>(k,c,v, d, i, b) {};
		
		static void Display(std::ostream& os)
		{
			Transactions.Display(os);
		}
		
		using TextSeparator = T::char_<' '> ;
		
		template<typename T>
		static std::string Extract(std::string s)
		{
			if(s == "\"Buchungstext\"")
				return "";
			
			auto vals = String_::Split<TextSeparator>(s);
						
			auto it = std::find_if (vals.begin(), vals.end(), [](auto s){return String_::Contains(s,T::Identifier); } );
			
			if(it == vals.end())
				return "";
						
			return *(++it);
		}
		
		static std::string ExtractKey(std::string s)
		{
			if(s == "\"Buchungstext\"")
				return "";
			
			auto vals = String_::Split<TextSeparator>(s);
			
			auto key = vals.at(1);
			std::string toErase = "Kto/IBAN";
			
			size_t pos = key.find(toErase);
			if (pos != std::string::npos)
			{
				// If found then erase it from string
				key.erase(pos, toErase.length());
			}
			
			return key;
		}
	};
	
	template<unsigned int N>
	struct Raiba: public AccountTransaction<Raiba<N>>
	{
		enum{ Num = N };
		using Type = Raiba<N>;
		inline static const std::string Name = "Raiba";
		inline static const std::string Filename = "Umsaetze_DE19660623660009232702.csv";
		inline static constexpr unsigned int KeyIdx = 4;
		inline static constexpr unsigned int CauseIdx = 9;
		inline static constexpr unsigned int DateIdx = 0;
		inline static constexpr unsigned int QuantityIdx = 12;
		
		inline static AccountTransaction<Type>::ParseCont Transactions = typename AccountTransaction<Type>::ParseCont();
		Raiba(std::string k, std::string c, double v, std::string d, std::string i = "IBAN", std::string b = "BIC") : AccountTransaction<Raiba<N>>(k,c,v, d, i, b) {};
		
		static void Display(std::ostream& os)
		{
			Transactions.Display(os);
		}
		
		template<typename T>
		static std::string Extract(std::string s)
		{
			return s;
		}
		static std::string ExtractKey(std::string s)
		{
			return s;
		}
	};
	
	template<unsigned int N = 0>
	struct Custom: public AccountTransaction<Custom<N>>
	{
		enum{ Num = N };
		using Type = Custom<N>;
		using Base = AccountTransaction<Type>;
		inline static const std::string Name = "Custom";
		inline static const std::string Filename = "RaibaKonten2021_1.csv";
		inline static constexpr unsigned int KeyIdx = 1;
		inline static constexpr unsigned int CauseIdx = 2;
		inline static constexpr unsigned int DateIdx = 0;
		inline static constexpr unsigned int QuantityIdx = 3;
		
		inline static AccountTransaction<Type>::ParseCont Transactions = typename AccountTransaction<Type>::ParseCont();
		Custom(std::string k, std::string c, double v, std::string d, std::string i = "IBAN", std::string b = "BIC") : AccountTransaction<Custom<N>>(k,c,v, d, i, b) {};		
		
		static void Display(std::ostream& os)
		{
			Transactions.Display(os);
		}
		
		template<typename T>
		static std::string Extract(std::string s)
		{
			return s;
		}
		
		static std::string ExtractKey(std::string s)
		{
			return s;
		}
	};
	template<typename T>
	std::ostream& operator<<(std::ostream& out, const AccountTransaction<T>& s)
	{
		out<<std::setw(30)<<std::left<<s.GetKey()<<std::setw(60)<<s.GetEntry()<<std::setw(20)<<std::right<<s.GetDate()<<std::setw(10)<<std::setprecision(2)<<std::fixed<<s.GetQuantity()<<"\n";
		return out<<std::setw(30)<<std::left<<s.GetIBAN()<<std::setw(60)<<s.GetBIC()<<std::setw(20);
	}
}


#endif
