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
#include "../Home/Parser.hpp"
#include "../String/Parser.hpp"

#ifndef RAIBA_HPP
#define RAIBA_HPP

namespace fs = std::filesystem;

namespace Bank
{	
	template<unsigned int N = 0, typename TransferT = std::tuple<IBAN,BIC,DateTimes::Date, Quantity<Sum>, Bank::Direction, Entry>>
	struct Raiba: public Account<Raiba<N>, TransferT>
	{
		enum{ Num = N };
		using TransferTypes = TransferT;
		using TransferType = Transfer<Raiba,TransferTypes>;
		using IsOutTransferSign = T::char_<'S'>;
		using Base = Account<Raiba, TransferTypes>;
		friend class Account<Raiba, TransferTypes>;
		
		inline static T::Is_<IsOutTransferSign> IsOutTransfer;
		inline static const std::string Name = "Raiba";
		inline static const std::string Filename = "Umsaetze_DE19660623660009232702";
				
		inline static Base::ParseContainer Cont = typename Base::ParseContainer();
		Raiba(std::string k, std::string c, double v, std::string d, std::string i = "IBAN", std::string b = "BIC") : Base(k,c,v, d, i, b) {};
		
		static void Display(std::ostream& os)
		{
			os<<"CONT"<<std::endl;
			Cont.Display(os);
		}
		
		static void ProcessValues(Base::InputIterator begin, Base::InputIterator end)
		{
			//~ auto keyLine = *(begin + OwnerIdx);
			//~ if(keyLine != "")
			//~ {
				//~ auto key = keyLine;
				//~ auto date = *(begin + DateIdx);
				//~ auto transaction = *(begin + TranactionIdx);
				
				//~ auto val = *(begin + QuantityIdx);
				//~ std::string::iterator end_pos = std::remove(val.begin(), val.end(), ' ');
				//~ val.erase(end_pos, val.end());

				
				//~ auto valString = *(end-2);
				//~ parser.Parse(transaction);
				//~ Logger::Log()<<"TRANSACTION:\n"<<transaction<<std::endl;
				//~ valString = String_::Remove<T::char_<','>>(valString);
				//~ valString = String_::Remove<T::char_<'.'>>(valString);
				
				//~ if(isdigit(valString.at(0)))
				//~ {
					//~ auto sum = std::stod(valString) / 100;
				
					//~ auto iban =  *(begin + IBANIdx);
					//~ auto bic =  *(begin + BICIdx);
					
					//~ std::string sign = *(end-1);
					//~ Base::InsertInContainer(key,transaction,sum, date, iban, bic,sign[0], transaction);
					//~ Logger::Log()<<"VAL:\n"<<valString<<std::endl;
				//~ }
			//~ }
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
		
		static void InterpretTransaction(std::string transaction)
		{
			;
		}
	private:
		inline static String_::Parser parser = String_::Parser();
	};
}


#endif
