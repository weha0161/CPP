#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <memory>
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
#include "Transfers.hpp"
#include "../Logger/Logger.hpp"
#include "../CSV/CSV.hpp"
#include "../Quantity/Quantity.h"
#include "../Typelist/Typelist.h"
#include "../Visitor/Visitor.hpp"
#include "../String/String_.hpp"
#include "../Common/DateTimes.hpp"

#ifndef ACCOUNTTRANSFER_HPP
#define ACCOUNTTRANSFER_HPP

namespace fs = std::filesystem;

namespace Bank
{
	template<typename Direction>
	struct Transfer
	{
		Transfer(std::string s){};
		using Type = Transfer<Direction>;
		using ValueType = Direction;
		inline static const std::string Identifier = "Direction";
		inline static const std::string Id = Direction::Id; 
		inline static constexpr int Value = Direction::Value; 		
	};
	
	struct In
	{
		using Type = In;
		inline static const std::string Id = "In"; 
		inline static constexpr int Value = 1; 
	};
	
	struct Out
	{
		using Type = Out;
		inline static const std::string Id = "Out"; 
		inline static constexpr int Value = -1; 
	};
	
	struct Unknown
	{
		using Type = Unknown;
		inline static const std::string Id = "Unknown"; 
		inline static constexpr int Value = 0; 
	};
	
	//-----------------------------------------------------------------------------------------------AccountTransfer-----------------------------------------------------------------------
	
	template<typename Account, typename TupleT,typename Direction>
	class AccountTransfer
	{
		using Type = AccountTransfer<Account,TupleT,Direction> ;
		
		Key<std::string> owner;
		Entry transaction;
		Entry cause;
		DateTimes::Date date;
		IBAN iban;
		BIC bic;
		Quantity<Sum> value;

	protected:
		using CSVSeparator = T::char_<';'> ;
	public:
		using TupleType = TupleT;
		using AccountType = Account ;
		using DirectionType = Direction ;
		using KeyType = Key<std::string>;
		using QunatityType = Quantity<Sum>;
		
		AccountTransfer(std::string k, std::string c, double v, std::string d, std::string i = "IBAN", std::string b = "BIC", std::string cause_ = "") : owner(k), transaction(c), date(Parsers::Parser<std::string,DateTimes::Date,std::string>::Parse(d)), value(v), iban(i), bic(b), cause(cause_) { 	};
		
		const KeyType& GetOwner() const { return owner; }
		const Entry& GetTransaction() const { return transaction; }
		const Entry& GetCause() const { return cause; }
		const DateTimes::Date& GetDate() const { return date; }
		const IBAN& GetIBAN() const { return iban; }
		const BIC& GetBIC() const { return bic; }
		const Quantity<Sum>& GetQuantity() const { return value; }
		const Direction& GetDirection() const { return Direction::Id; }		

		bool operator==(DayType d) const{ return this->date == d;};
		bool operator==(DateTimes::Day m) const{ return this->date == m;};
		bool operator==(MonthType m) const{ return this->date == m;};
		bool operator==(DateTimes::Month m) const{ return this->date == m;};
		bool operator==(YearType y) const{ return this->date == y;};
		bool operator==(DateTimes::Year y) const{ return this->date == y;};
		bool operator==(DateTimes::Date date) const{ return this->date == date;};
		bool operator==(DateType date) const{ return this->date == date;};
	};
}


#endif
