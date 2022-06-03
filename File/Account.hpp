#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <ctime>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <exception>
#include <unordered_map>
#include <boost/mpl/vector.hpp>
#include <boost/filesystem.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/placeholders.hpp>
#include <filesystem>
#include "TransferItemContainer.hpp"
#include "JSONParser.hpp"
#include "../Logger/Logger.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../CSV/CSV.hpp"
#include "../Quantity/Quantity.h"
#include "../Typelist/Typelist.h"
#include "../Visitor/Visitor.hpp"
#include "../String/String_.hpp"
#include "../CSV/KeyIndex.hpp"

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
		Key<std::string> owner;
		IBAN iban;
		BIC bic;
		
		inline static const std::string  KeysFilename = Derived::Name + ".keys";
	public:
		using Type = Account<Derived> ;
		using InTransfer = AccountTransfer<Derived,Transfer<In>>;
		using OutTransfer = AccountTransfer<Derived, Transfer<Out>>;
		using KeyType = Key<std::string>;
		using ParseContIn = TransferContainer<AccountTransfer<Derived,Transfer<In>>>;
		using ParseContOut = TransferContainer<AccountTransfer<Derived,Transfer<Out>>>;
		using QunatityType = Quantity<Sum>;
		using InputIterator = std::vector<std::string>::const_iterator;
		using KeyIndexType = CSV::KeyIndex<KeyType,uint>;
		using KeyIndexContainerType = CSV::KeyIndexContainer<Derived, std::string,uint>;
		using KeyIndexContainerPtrType = std::unique_ptr<KeyIndexContainerType>;
		
		static void Parse(InputIterator begin, InputIterator end)
		{
			if(keyIndices->Empty())
			{
	 			Logger::Log<Error>()<<Derived::Name<<" parsing not possible, no keys!"<<std::endl;
				return;
			}
			
			if(begin != end)
			{
	 			Logger::Log<Error>()<<Derived::Name<<" Before"<<std::endl;
	 			keyIndices->Display(std::cout);
				for(auto it = begin;it != end; ++it)
				{
					auto values = String_::Split<CSVSeparator>(String_::Remove<String_::Newline>(*it));
					
					if(keyIndices->UpdateKeys(values))
					{
						Logger::Log()<<"KEYLINE"<<*it<<std::endl;
			 			Logger::Log<Error>()<<Derived::Name<<" After"<<std::endl;
						keyIndices->Display(std::cout);
						
						Logger::Log<Error>()<<*(values.cbegin()+6)<<std::endl;
						
						return;
						//~ Derived::ProcessValues(values.cbegin(), values.cend());					
					}
					//~ if (values.size() < MaxIdx)
						//~ continue;
										
				}
			}

			return;
		}
		
		
		static void ReadKeyPatterns(InputIterator begin, InputIterator end)
		{
			TransferItemContainerType::Instance().Read();
			if(begin==end)
			{
	 			Logger::Log<Error>()<<"ReadKeyPatterns: No Items found for "<<Derived::Name<<std::endl;
	 			// Todo weiter Fehlerbehandlung für weiter Bearbeitung
	 			return;
			}
						
			for(auto it = begin; it != end; ++it)
			{
				try
				{
					auto values = String_::Split<T::char_<':'>>(*it);
					auto keyItem = *values.cbegin();
					auto keys = String_::Split<T::char_<';'>>(String_::Remove<String_::CR>(*(values.cbegin()+1)));
		 			
		 			if(keys.cbegin() == keys.cend())
			 			Logger::Log<Error>()<<Derived::Name<<" ReadKeyPatterns: No keys found for item"<<keyItem<<std::endl;
					else
						keyIndices->UpdateKeyPatterns(Key(keyItem), keys);
				}
				catch(std::exception e)
				{
		 			Logger::Log<Error>()<<"ReadKeyPatterns: "<<Derived::Name<<"\t"<<e.what()<<std::endl;
				}
			}
		}
				
		template<typename Cont>
		static void RegisterTo(Cont& cont)
		{
			cont.insert(std::make_pair(Derived::Filename,  &Type::Parse));
			cont.insert(std::make_pair(Type::KeysFilename,  &Type::ReadKeyPatterns));
		}	
		
	protected:
		Account(std::string k, std::string c, double v, std::string d, std::string i = "IBAN", std::string b = "BIC") : owner(k), iban(i), bic(b) { };
		
		static constexpr unsigned int Indices[4] = {Derived::OwnerIdx, Derived::DateIdx, Derived::TranactionIdx, Derived::QuantityIdx};
		static const unsigned int MaxIdx = *std::max_element(Indices,Indices+4);
		inline static KeyIndexContainerPtrType keyIndices = std::make_unique<KeyIndexContainerType>(TransferItemContainerType::Instance().template Create<Derived>());
		
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
			if(Derived::IsOutTransfer(transferSign))
				Derived::OutCont.Insert(key, std::make_shared<OutTransfer>(key,transaction,sum, date, iban, bic, cause));
			else
				Derived::InCont.Insert(key,  std::make_shared<InTransfer>(key,transaction,sum, date, iban, bic, cause));
			
		}
	private:
		static uint findKeyLine(InputIterator begin, InputIterator end)
		{
			uint ret = 0;
			for(auto line = begin; (line + ret) != end; ++ret)
			{
				if(String_::Contains(*(line + ret), "IBAN"))
				{
					return ret;
				}
			}
			
			return ret;
		}
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
