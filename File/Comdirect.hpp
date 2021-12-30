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
	template<unsigned int N = 0>
	struct Comdirect: public Account<Comdirect<N>>
	{
		enum{ Num = N };
		using InType = AccountTransfer<Comdirect,Transfer<In>>;
		using OutType = AccountTransfer<Comdirect,Transfer<Out>>;
		using IsOutTransferSign = T::char_<'-'>;
		using Base = Account<Comdirect>;
		
		inline static T::Is_<IsOutTransferSign> IsOutTransfer;
		inline static const std::string Name = "Comdirect";
		inline static const std::string Filename = "Umsaetze_1026947527.csv";
		inline static constexpr unsigned int OwnerIdx = 3;
		inline static constexpr unsigned int TranactionIdx = 3;
		inline static constexpr unsigned int DateIdx = 0;
		inline static constexpr unsigned int QuantityIdx = 4;
		inline static constexpr unsigned int HeaderLength = 5;
		inline static constexpr unsigned int TrailerLength = 23;
		
		inline static Base::ParseContIn InCont = typename Base::ParseContIn();
		inline static Base::ParseContOut OutCont = typename Base::ParseContOut();
		Comdirect(std::string k, std::string c, double v, std::string d, std::string i = "IBAN", std::string b = "BIC") : Base(k,c,v, d, i, b) {};
		
		static void Display(std::ostream& os)
		{
			os<<"IN"<<std::endl;
			InCont.Display(os);
			
			os<<"OUT"<<std::endl;
			OutCont.Display(os);
		}
		
		using TextSeparator = T::char_<' '> ;
		
		static void ProcessValues(std::vector<std::string> values)
		{
			auto keyLine = values.at(OwnerIdx);
			if(keyLine != "")
			{
				auto key = ExtractKey(keyLine);
				auto date = values.at(DateIdx);
				auto transaction = values.at(TranactionIdx);
				
				auto n = Base::GetNumericValue(values.at(QuantityIdx));
				auto sum = n != "" ? std::stod(n) : 0.0 ;
			
				auto iban =  Extract<IBAN>(transaction);
				auto bic = Extract<BIC>(transaction);
								
				Base::InsertInContainer(key,transaction,sum, date, iban, bic, *(values.at(QuantityIdx).begin()+1), String_::Remove<T::char_<'"'>>(transaction));
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
