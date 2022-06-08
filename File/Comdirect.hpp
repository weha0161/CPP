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
#include "Account.hpp"
#include "../Logger/Logger.hpp"
#include "../CSV/CSV.hpp"
#include "../Quantity/Quantity.h"
#include "../Typelist/Typelist.h"
#include "../Visitor/Visitor.hpp"
#include "../String/String_.hpp"
#include "../Home/Parser.hpp"

#ifndef COMDIRECT_HPP
#define COMDIRECT_HPP

namespace fs = std::filesystem;

namespace Bank
{
	//-----------------------------------------------------------------------------------------------Tranfers-----------------------------------------------------------------------
	template<unsigned int N = 0, typename TransferT = std::tuple<IBAN,BIC,DateTimes::Date, Quantity<Sum>, Bank::Direction<Bank::Unknown>>>
	struct Comdirect: public Account<Comdirect<N>, TransferT>
	{
		enum{ Num = N };
		using TransferTypes = TransferT;
		using TransferType = Transfer<Comdirect,TransferTypes,Direction<Comdirect>>;
		using IsOutTransferSign = T::char_<'-'>;
		using Base = Account<Comdirect, TransferTypes>;
		
		inline static T::Is_<IsOutTransferSign> IsOutTransfer;
		inline static const std::string Name = "Comdirect";
		inline static const std::string Filename = "Umsaetze_DE832004113306947527";
		inline static constexpr unsigned int OwnerIdx = 3;
		inline static constexpr unsigned int TranactionIdx = 3;
		inline static constexpr unsigned int DateIdx = 0;
		inline static constexpr unsigned int QuantityIdx = 4;
		inline static constexpr unsigned int HeaderLength = 5;
		inline static constexpr unsigned int TrailerLength = 23;
		
		inline static Base::ParseContainer Cont = typename Base::ParseContainer();
		Comdirect(std::string k, std::string c, double v, std::string d, std::string i = "IBAN", std::string b = "BIC") : Base(k,c,v, d, i, b) {};
		
		static void Display(std::ostream& os)
		{
			os<<"IN"<<std::endl;
			Cont.Display(os);
		}
		
		using TextSeparator = T::char_<' '> ;
		
		static void ProcessValues(Base::InputIterator begin, Base::InputIterator end)
		{
			auto keyLine = *(begin + OwnerIdx);
			if(keyLine != "")
			{
				auto key = ExtractKey(keyLine);
				auto date = *(begin + DateIdx);
				auto transaction = *(begin + TranactionIdx);
				
				auto n = Base::GetNumericValue(*(begin + QuantityIdx));
				auto sum = n != "" ? std::stod(n) : 0.0 ;
			
				auto iban =  Extract<IBAN>(transaction);
				auto bic = Extract<BIC>(transaction);
								
				auto q = std::string(*(begin + QuantityIdx));
				Base::InsertInContainer(key,transaction,sum, date, iban, bic, *(q.cbegin()+1), String_::Remove<T::char_<'"'>>(transaction));
			}
				
		}		
		
	protected:
		template<typename T>
		static std::string Extract(std::string s)
		{
			Parser::JSON json;
			auto vals = json.Parse(s);

			auto it = std::find_if (vals.begin(),vals.end(), [](std::pair<std::string, std::string> const& item) { return String_::Contains(item.first,T::Identifier); } );
			
			if(it == vals.end())
				return "";
						
			return (it)->second;
		}
		
		static std::string ExtractKey(std::string s)
		{
			Parser::JSON json;
			if(s == "\"Buchungstext\"")
				return "";
			
			auto vals = json.Parse(s);
			
			return vals.begin()->second;
		}
		
		
	};
}


#endif
