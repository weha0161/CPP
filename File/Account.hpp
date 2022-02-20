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
#include "JSONParser.hpp"
#include "../Logger/Logger.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../CSV/CSV.hpp"
#include "../Quantity/Quantity.h"
#include "../Typelist/Typelist.h"
#include "../Visitor/Visitor.hpp"
#include "../String/String_.hpp"
// #include "../Home/Parser.hpp"

#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

namespace fs = std::filesystem;

namespace Bank
{
	template<typename Derived>
	class Account
	{
	protected:		
		using CSVSeparator = T::char_<';'> ;
		Key owner;
		IBAN iban;
		BIC bic;
	public:
		using Type = Account<Derived> ;
		using InTransfer = AccountTransfer<Derived,Transfer<In>>;
		using OutTransfer = AccountTransfer<Derived, Transfer<Out>>;
		using KeyType = Key ;
		using ParseContIn = TransferContainer<AccountTransfer<Derived,Transfer<In>>>;
		using ParseContOut = TransferContainer<AccountTransfer<Derived,Transfer<Out>>>;
		using QunatityType = Quantity<Sum>;
		using InputIterator = std::vector<std::string>::const_iterator;
		
		static void Parse(InputIterator begin, InputIterator end)
		{
			if(begin != end)
			{
				uint ctr = 0;
							
				auto lines = std::vector<std::string>(begin + Derived::HeaderLength, end - Derived::TrailerLength);
				for(auto line : lines)
				{
					auto values = String_::Split<CSVSeparator>(line);
					
					if (values.size() < MaxIdx)
						continue;
										
					Derived::ProcessValues(values.cbegin(), values.cend());					
				}
			}

			return;
		}
		
				
		template<typename Cont>
		static void RegisterTo(Cont& cont)
		{
			cont.insert(std::make_pair(Derived::Filename,  &Type::Parse));
		}	
		
	protected:
		Account(std::string k, std::string c, double v, std::string d, std::string i = "IBAN", std::string b = "BIC") : owner(k), iban(i), bic(b) { };
		
		static constexpr unsigned int Indices[4] = {Derived::OwnerIdx, Derived::DateIdx, Derived::TranactionIdx, Derived::QuantityIdx};
		static const unsigned int MaxIdx = *std::max_element(Indices,Indices+4);
		
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
		
		static void InsertInContainer(std::string key, std::string transaction, double sum, std::string date, std::string iban, std::string bic, char transferSign, std::string cause = "")
		{
// 			Logger::Log()<<date<<"\t"<<sum<<"\t"<<iban<<std::endl;
			if(Derived::IsOutTransfer(transferSign))
				Derived::OutCont.Insert(key, std::make_shared<OutTransfer>(key,transaction,sum, date, iban, bic, cause));
			else
				Derived::InCont.Insert(key,  std::make_shared<InTransfer>(key,transaction,sum, date, iban, bic, cause));
			
		}
		
	private:
	};
}

template<typename A, typename Direction>
struct Get{};

template<typename A>
struct Get<A, Bank::Out>
{
	Bank::AccountEndpoint<A, Bank::Transfer<Bank::Out>> operator()(typename A::KeyType k)
	{
		return A::OutCont[k];
	}
};

template<typename A>
struct Get<A, Bank::In>
{
	Bank::AccountEndpoint<A, Bank::Transfer<Bank::In>> operator()(typename A::KeyType k)
	{
		return A::InCont[k];
	}
};

template<typename A, typename Direction = Bank::Out>
struct Transfers
{
	const typename A::ParseContOut& operator()()
	{
		return A::OutCont;
	}
};

template<typename A>
struct Transfers<A, Bank::In>
{
	const typename A::ParseContIn& operator()()
	{
		return A::InCont;
	}
};

#endif
