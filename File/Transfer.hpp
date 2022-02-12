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

#ifndef TRANSFER_HPP
#define TRANSFER_HPP

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
	
	//-----------------------------------------------------------------------------------------------TranferEndpoint-----------------------------------------------------------------------
	
	template<typename Account, typename Direction, template<typename> class Cont = std::vector>
	class AccountEndpoint
	{
		using Type = AccountEndpoint<Account,Direction> ;
		using TransferType = AccountTransfer<Account,Direction> ;
		using DataType = std::shared_ptr<TransferType>;
		using ResultContainer = Cont<DataType> ;
		using ContainerType = TransactionContainer<DataType>;
		using Iterator = ContainerType::Iterator;
		
		Key owner;
		IBAN iban;
		BIC bic;
		Quantity<Sum> total;
		ContainerType transactions;

	protected:
		using CSVSeparator = T::char_<';'> ;
	public:
		using KeyType = Key;
		using QunatityType = Quantity<Sum>;
		
		AccountEndpoint(std::string ownerKey, std::string i = "IBAN", std::string b = "BIC") : owner(ownerKey), iban(i), bic(b) { };
		AccountEndpoint(const DataType t) : owner(t->GetOwner()), iban(t->GetIBAN()), bic(t->GetBIC()), total(t->GetQuantity()) 
		{ 
			this->transactions = ContainerType();
			this->transactions.Add(t);
		};
		AccountEndpoint():owner("ownerKey"), iban("i"), bic("b"), total(0) { };
		
		const Key& GetOwner() const { return owner; }
		const IBAN& GetIBAN() const { return iban; }
		const BIC& GetBIC() const { return bic; }
		const Quantity<Sum>& GetTotal() const { return total; }
		const Direction& GetDirection() const { return Direction::Id; }		
		
		void Add(DataType t)
		{
			this->transactions.Add(t);
			this->total = this->total + t->GetQuantity();
		}
		
		void Display(std::ostream& out)
		{
			out<<"Owner: "<<owner<<"\tIBAN: "<<iban<<"\tBIC: "<<bic<<std::endl;
			for(auto it = this->transactions.Begin(); it != this->transactions.End(); ++it)
			{
				out<<"\tDate: "<<(*it)->GetDate()<<"\tSum: "<<std::setprecision(2)<<std::fixed<<(*it)->GetQuantity()<<std::endl;
				out<<"\t"<<"\t"<<(*it)->GetCause()<<std::endl;
			}

			out<<std::endl;
		}
		
		ResultContainer GetCause(std::string name = "")
		{
			ResultContainer result;
			for(auto it = this->transactions.Begin(); it != this->transactions.End(); ++it)
			{
				if(String_::Contains((*it)->GetCause().Value, name))
					result.push_back(*it);
			}
			
			return result;
		}
		
		const ContainerType& Transactions() const { return this->transactions; }
	};
	
	//-----------------------------------------------------------------------------------------------TranferContainer-----------------------------------------------------------------------
		
	template<typename T, template<typename, typename> class TCont = std::map, template<typename> class Cont = std::vector>
	class TransferContainer
	{
	public:
		using KeyType = typename T::KeyType;
		using DataType = std::shared_ptr<T>;
		using AccountEndpointType = AccountEndpoint<typename T::AccountType, typename T::DirectionType>;
	private:
		Cont<KeyType> keys;
		TCont<KeyType, AccountEndpointType> transfers;
	public:
		void Insert(KeyType k, DataType t)
		{
			if(!this->Contains(k))
			{
				this->keys.push_back(k);
				this->transfers.insert(std::pair<KeyType, AccountEndpointType>(k,AccountEndpointType(t)));
				return;
			}
			
			this->transfers[k].Add(t);
		}
		
		bool Contains(KeyType k) const { return this->transfers.find(k) != this->transfers.end(); }
		const Cont<KeyType>& Keys() { return keys; }
		const AccountEndpointType& operator[](KeyType k) { return this->transfers[k]; }
				
		void Display(std::ostream& out) const
		{
			for(auto p : this->transfers)
			{
				out<<p.first<<":"<<std::endl;
				
				p.second.Display(out);
				
				out<<"\nTotal: "<<std::setprecision(2)<<std::fixed<<p.second.GetTotal()<<std::endl;
				
				out<<std::endl;
			}
		}
		
		void Display(std::ostream& out, const std::vector<std::string>& keys)
		{
			for(auto k : keys)
			{
				const KeyType key = KeyType(k);
				if(!this->Contains(key))
					continue;
				
				auto p = this->transfers[key];				
				p.Display(out);
				
				out<<"\nTotal: "<<std::setprecision(2)<<std::fixed<<p.GetTotal()<<std::endl;
				out<<std::endl;
			}
		}
		
		void DisplayKeys(std::ostream& out) const
		{
			for(auto p : this->transfers)
			{
				out<<p.first<<std::endl;				
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
