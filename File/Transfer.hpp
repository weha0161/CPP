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

#ifndef TRANSFER_HPP
#define TRANSFER_HPP

namespace fs = std::filesystem;

namespace Bank
{
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
	
	template<typename AccountT>
	struct DirectionBase: public Element
	{
		DirectionBase(std::string s): Element(s) {};
		using Type = DirectionBase<AccountT>;
		using AccountType = AccountT;
		inline static const std::string Identifier = "Direction";
		inline static const std::string Id = Unknown::Id; 
		inline static constexpr int Value = Unknown::Value; 		
		DirectionBase* DoCreate(){return this;};
	};
	
	template<typename AccountT>
	struct Direction: public DirectionBase<AccountT>
	{
		Direction(std::string s): DirectionBase<AccountT>(s){};
	};
	
	//-----------------------------------------------------------------------------------------------Transfer-----------------------------------------------------------------------
	
	template<typename Account, typename TupleT>
	class Transfer
	{
		using Type = Transfer<Account,TupleT> ;
		
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
		using KeyType = Key<std::string>;
		using QunatityType = Quantity<Sum>;
		
		Transfer(std::string k, std::string c, double v, std::string d, std::string i = "IBAN", std::string b = "BIC", std::string cause_ = "") : owner(k), transaction(c), date(Parsers::Parser<std::string,DateTimes::Date,std::string>::Parse(d)), value(v), iban(i), bic(b), cause(cause_) { 	};
		
		const KeyType& GetOwner() const { return owner; }
		const Entry& GetTransaction() const { return transaction; }
		const Entry& GetCause() const { return cause; }
		const DateTimes::Date& GetDate() const { return date; }
		const IBAN& GetIBAN() const { return iban; }
		const BIC& GetBIC() const { return bic; }
		const Quantity<Sum>& GetQuantity() const { return value; }
		const auto& GetDirection() const { return Direction<Unknown>::Id; }		

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
