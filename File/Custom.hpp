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
#include "Direction.hpp"
#include "../Logger/Logger.hpp"
#include "../CSV/CSV.hpp"
#include "../Quantity/Quantity.h"
#include "../Typelist/Typelist.h"
#include "../Visitor/Visitor.hpp"
#include "../String/String_.hpp"

#ifndef CUSTOM_HPP
#define CUSTOM_HPP

namespace fs = std::filesystem;

namespace Bank
{
	template<unsigned int N = 0, typename TransferT = std::tuple<IBAN,BIC,DateTimes::Date, Quantity<Sum>, Bank::Direction, Entry>>
	struct Custom: public Account<Custom<N>, TransferT>
	{
		enum{ Num = N };
		using TransferTypes = TransferT;
		using TransferType = Transfer<Custom,TransferTypes>;
		using IsOutTransferSign = T::char_<'-'>;
		using Base = Account<Custom, TransferTypes>;
		
		inline static T::Is_<IsOutTransferSign> IsOutTransfer;
		inline static const std::string Name = "Custom";
		inline static const std::string Filename = "RaibaKonten2021_1.csv";

		inline static Base::ParseContainer Cont = typename Base::ParseContainer();
		Custom(std::string k, std::string c, double v, std::string d, std::string i = "IBAN", std::string b = "BIC") : Base(k,c,v, d, i, b) {};		
		
		static void Display(std::ostream& os)
		{
			Cont.Display(os);
		}
		
		static void ProcessValues(Base::InputIterator begin, Base::InputIterator end)
		{
			//~ auto keyLine = *(begin + OwnerIdx);
			//~ if(keyLine != "")
			//~ {
				//~ auto key = ExtractKey(keyLine);
				//~ auto date = *(begin + DateIdx);
				//~ auto transaction = *(begin + TranactionIdx);
				
				//~ auto n = Base::GetNumericValue(*(begin + QuantityIdx));
				//~ auto sum = n != "" ? std::stod(n) : 0.0 ;
			
				//~ auto iban =  Extract<IBAN>(transaction);
				//~ auto bic = Extract<BIC>(transaction);

				//~ Cont.Insert(key, std::make_shared<typename Base::InTransfer>(key,transaction,sum, date, iban, bic));
				
				//~ auto q = std::string(*(begin + QuantityIdx));
				//~ Base::InsertInContainer(key,transaction,sum, date, iban, bic, *(q.cbegin()+1));
			//~ }
		}				
				
	protected:
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
}


#endif
