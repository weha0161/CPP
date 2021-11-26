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
#include "Transfer.hpp"
#include "../Logger/Logger.hpp"
#include "../CSV/CSV.hpp"
#include "../Quantity/Quantity.h"
#include "../Typelist/Typelist.h"
#include "../Visitor/Visitor.hpp"
#include "../String/String_.hpp"

#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

namespace fs = std::filesystem;

namespace Bank
{
	template<typename Derived>
	class Account
	{
		using Type = Account<Derived> ;
	protected:		
// 		inline static T::Is_<typename Derived::IsOutTransferSign> IsOutTransfer;
		using CSVSeparator = T::char_<';'> ;
	public:
		Key owner;
		IBAN iban;
		BIC bic;
		using InTransfer = AccountTransfer<Derived,Transfer<In>>;
		using OutTransfer = AccountTransfer<Derived, Transfer<Out>>;
		using KeyType = Key ;
		using ParseContIn = TransferContainer<AccountTransfer<Derived,Transfer<In>>>;
		using ParseContOut = TransferContainer<AccountTransfer<Derived,Transfer<Out>>>;
		using QunatityType = Quantity<Sum>;
		
		Account(std::string k, std::string c, double v, std::string d, std::string i = "IBAN", std::string b = "BIC") : owner(k), iban(i), bic(b) { };
		
		static constexpr unsigned int Indices[4] = {Derived::OwnerIdx, Derived::DateIdx, Derived::TranactionIdx, Derived::QuantityIdx};
		static const unsigned int MaxIdx = *std::max_element(Indices,Indices+4);
		
		static void Parse(std::vector<std::string> content)
		{
			if(content.size() > 0)
			{
				uint ctr = 0;
							
// 				Logger::Log(content.cbegin(), content.cend());
				
				auto header = RemoveHeader(content);
				auto trailer = RemoveTrailer(content);
	
				for(auto line : content)
				{
					auto values = String_::Split<CSVSeparator>(line);
					
					if (values.size() < MaxIdx)
						continue;
										
					Derived::ProcessValues(values);
					
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
		
		static std::vector<std::string> RemoveHeader(std::vector<std::string>&  cont)
		{
			std::vector<std::string> header;
			std::copy(cont.begin(), cont.begin() + Derived::HeaderLength, std::back_inserter(header));
			cont.erase(cont.begin(), cont.begin() + Derived::HeaderLength);
			return header;
		}	
		
		static std::vector<std::string> RemoveTrailer(std::vector<std::string>&  cont)
		{
			std::vector<std::string> header;
			std::copy(cont.end() - Derived::TrailerLength, cont.end(), std::back_inserter(header));
			cont.erase(cont.end() - Derived::TrailerLength, cont.end());
			return header;
		}	
		
	protected:
		static void InsertInContainer(std::string key, std::string transaction, double sum, std::string date, std::string iban, std::string bic, char transferSign)
		{
			if(Derived::IsOutTransfer(transferSign))
				Derived::OutCont.Insert(key, OutTransfer(key,transaction,sum, date, iban, bic));
			else
				Derived::InCont.Insert(key, InTransfer(key,transaction,sum, date, iban, bic));
			
		}
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
			auto vals = String_::Split<JSON_Separator>(input);
				
			return Parse(*vals.begin(), ++vals.begin(),vals.end());
		}
	};
}


#endif
