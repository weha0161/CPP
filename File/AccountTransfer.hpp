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
#include "TransactionContainer.hpp"
#include "../Logger/Logger.hpp"
#include "../CSV/CSV.hpp"
#include "../Quantity/Quantity.h"
#include "../Typelist/Typelist.h"
#include "../Visitor/Visitor.hpp"
#include "../String/String_.hpp"

#ifndef ACCOUNTTRANSFER_HPP
#define ACCOUNTTRANSFER_HPP

namespace fs = std::filesystem;

namespace Bank
{
	template<typename Direction>
	struct Transfer
	{
		using Type = Transfer<Direction>;
		using ValueType = Direction;
		inline static const std::string Id = Direction::Id;
	};
	
	struct In
	{
		using Type = In;
		inline static const std::string Id = "In"; 
	};
	
	struct Out
	{
		using Type = Out;
		inline static const std::string Id = "Out"; 
	};
	
	//-----------------------------------------------------------------------------------------------AccountTransfer-----------------------------------------------------------------------
	
	template<typename Account, typename Direction>
	class AccountTransfer
	{
		using Type = AccountTransfer<Account,Direction> ;
		
		Key owner;
		Entry transaction;
		Entry cause;
		Date date;
		IBAN iban;
		BIC bic;
		Quantity<Sum> value;

	protected:
		using CSVSeparator = T::char_<';'> ;
	public:
		using AccountType = Account ;
		using DirectionType = Direction ;
		using KeyType = Key;
		using QunatityType = Quantity<Sum>;
		
		AccountTransfer(std::string k, std::string c, double v, std::string d, std::string i = "IBAN", std::string b = "BIC", std::string cause_ = "") : owner(k), transaction(c), date(d), value(v), iban(i), bic(b), cause(cause_) 
		{ 
			Logger::Log()<<this->cause<<std::endl;
		};
		
		const Key& GetOwner() const { return owner; }
		const Entry& GetTransaction() const { return transaction; }
		const Entry& GetCause() const { return cause; }
		const Date& GetDate() const { return date; }
		const IBAN& GetIBAN() const { return iban; }
		const BIC& GetBIC() const { return bic; }
		const Quantity<Sum>& GetQuantity() const { return value; }
		const Direction& GetDirection() const { return Direction::Id; }		
	};
}


#endif
