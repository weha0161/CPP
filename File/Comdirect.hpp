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

#ifndef COMDIRECT_HPP
#define COMDIRECT_HPP

namespace fs = std::filesystem;

namespace FS
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
		
		template<typename T>
		static std::string Extract(std::string s)
		{
			JSONParser json;
			auto vals = json.Parse(s);

			auto it = std::find_if (vals.begin(),vals.end(), [](std::pair<std::string, std::string> const& item) { return String_::Contains(item.first,T::Identifier); } );
			
			if(it == vals.end())
				return "";
						
			return (it)->second;
		}
		
		static std::string ExtractKey(std::string s)
		{
			JSONParser json;
			if(s == "\"Buchungstext\"")
				return "";
			
			auto vals = json.Parse(s);
			
			return vals.begin()->second;
		}
		
		
		
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
								
				Base::InsertInContainer(key,transaction,sum, date, iban, bic, *(values.at(QuantityIdx).begin()+1));
			}
				
		}
	};
	
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
			JSONParser json;
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
	
	template<unsigned int N = 0>
	struct Custom: public Account<Custom<N>>
	{
		enum{ Num = N };
		using InType = AccountTransfer<Custom,Transfer<In>>;
		using OutType = AccountTransfer<Custom,Transfer<Out>>;
		using IsOutTransferSign = T::char_<'-'>;
		using Base = Account<Custom>;
		
		inline static T::Is_<IsOutTransferSign> IsOutTransfer;
		inline static const std::string Name = "Custom";
		inline static const std::string Filename = "RaibaKonten2021_1.csv";
		inline static constexpr unsigned int OwnerIdx = 1;
		inline static constexpr unsigned int TranactionIdx = 2;
		inline static constexpr unsigned int DateIdx = 0;
		inline static constexpr unsigned int QuantityIdx = 3;
		inline static constexpr unsigned int HeaderLength = 15;
		inline static constexpr unsigned int TrailerLength = 4;
		
		inline static Base::ParseContIn InCont = typename Base::ParseContIn();
		inline static Base::ParseContOut OutCont = typename Base::ParseContOut();
		Custom(std::string k, std::string c, double v, std::string d, std::string i = "IBAN", std::string b = "BIC") : Base(k,c,v, d, i, b) {};		
		
		static void Display(std::ostream& os)
		{
			InCont.Display(os);
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

				InCont.Insert(key, typename Base::InTransfer(key,transaction,sum, date, iban, bic));
				OutCont.Insert(key, typename Base::OutTransfer(key,transaction,sum, date, iban, bic));
				
				Base::InsertInContainer(key,transaction,sum, date, iban, bic, *(values.at(QuantityIdx).begin()+1));
			}
		}				
				

	};
	template<typename T, typename D>
	std::ostream& operator<<(std::ostream& out, const AccountTransfer<T,D>& s)
	{
		out<<std::setw(30)<<std::left<<s.GetOwner()<<std::setw(60)<<s.GetTransaction()<<std::setw(20)<<std::right<<s.GetDate()<<std::setw(10)<<std::setprecision(2)<<std::fixed<<s.GetQuantity()<<"\n";
		return out<<std::setw(30)<<std::left<<s.GetIBAN()<<std::setw(60)<<s.GetBIC()<<std::setw(20);		
	}
}


#endif
