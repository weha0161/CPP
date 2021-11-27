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
#include "JSONParser.hpp"
#include "../Logger/Logger.hpp"
#include "../CSV/CSV.hpp"
#include "../Quantity/Quantity.h"
#include "../Typelist/Typelist.h"
#include "../Visitor/Visitor.hpp"
#include "../String/String_.hpp"

#ifndef RAIBA_HPP
#define RAIBA_HPP

namespace fs = std::filesystem;

namespace Bank
{	
	template<unsigned int N = 0>
	struct Raiba: public Account<Raiba<N>>
	{
		enum{ Num = N };
		using InType = AccountTransfer<Raiba,Transfer<In>>;
		using OutType = AccountTransfer<Raiba,Transfer<Out>>;
		using IsOutTransferSign = T::char_<'S'>;
		using Base = Account<Raiba>;
		
		
		inline static T::Is_<IsOutTransferSign> IsOutTransfer;
		inline static const std::string Name = "Raiba";
		inline static const std::string Filename = "Umsaetze_DE19660623660009232702.csv";
		inline static constexpr unsigned int OwnerIdx = 4;
		inline static constexpr unsigned int TranactionIdx = 9;
		inline static constexpr unsigned int IBANIdx = 6;
		inline static constexpr unsigned int BICIdx = 7;
		inline static constexpr unsigned int DateIdx = 0;
		inline static constexpr unsigned int QuantityIdx = 12;
		inline static constexpr unsigned int HeaderLength = 16;
		inline static constexpr unsigned int TrailerLength = 3;
		
		inline static Base::ParseContIn InCont = typename Base::ParseContIn();
		inline static Base::ParseContOut OutCont = typename Base::ParseContOut();
		Raiba(std::string k, std::string c, double v, std::string d, std::string i = "IBAN", std::string b = "BIC") : Base(k,c,v, d, i, b) {};
		
		static void Display(std::ostream& os)
		{
			os<<"IN"<<std::endl;
			InCont.Display(os);
			
			os<<"OUT"<<std::endl;
			OutCont.Display(os);
		}
		
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
			return s;
		}
		
		static void ProcessValues(std::vector<std::string> values)
		{
			auto keyLine = values.at(OwnerIdx);
			if(keyLine != "")
			{
				auto key = ExtractKey(keyLine);
				auto date = values.at(DateIdx);
				auto transaction = values.at(TranactionIdx);
				
				auto val = values.at(QuantityIdx);
				std::string::iterator end_pos = std::remove(val.begin(), val.end(), ' ');
				val.erase(end_pos, val.end());

				auto sum = std::stod(*(values.end()-2));
			
				auto iban =  values.at(IBANIdx);
				auto bic =  values.at(BICIdx);
				
				std::string sign = *(values.end()-1);
				Base::InsertInContainer(key,transaction,sum, date, iban, bic,sign[0]);
			}
		}				
	};
}


#endif
