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
#include "Transfer.hpp"
#include "Direction.hpp"
#include "../Logger/Logger.hpp"
#include "../CSV/CSV.hpp"
#include "../Quantity/Quantity.h"
#include "../Typelist/Typelist.h"
#include "../Visitor/Visitor.hpp"
#include "../String/String_.hpp"


#ifndef TRANSFERCONTAINER_HPP
#define TRANSFERCONTAINER_HPP

namespace fs = std::filesystem;

namespace Bank
{
	//-----------------------------------------------------------------------------------------------TranferEndpoint-----------------------------------------------------------------------
	
	template<typename Account, template<typename> class Cont = std::vector>
	class AccountEndpoint
	{
		using TransferTypes = Account::TupleType;
		using Type = AccountEndpoint<Account> ;
		using TransferType = Transfer<Account,TransferTypes> ;
		using DataType = std::shared_ptr<TransferType>;
		using ResultContainer = Cont<DataType> ;
		using ContainerType = Transfers<DataType>;
		using Iterator = ContainerType::Iterator;
		
		Key<std::string> owner;
		IBAN iban;
		BIC bic;
		Quantity<Sum> total;
		Direction direction;
		ContainerType transactions;

	protected:
		using CSVSeparator = T::char_<';'> ;
	public:
		using KeyType = Key<std::string>;
		using QunatityType = Quantity<Sum>;
		
		AccountEndpoint(std::string ownerKey, std::string i = "IBAN", std::string b = "BIC") : owner(ownerKey), iban(i), bic(b) { };
		AccountEndpoint(const DataType t) : owner(t->GetOwner()), iban(t->GetIBAN()), bic(t->GetBIC()), total(t->GetQuantity()) 
		{ 
			this->transactions = ContainerType();
			this->transactions.Add(t);
		};
		AccountEndpoint():owner("ownerKey"), iban("i"), bic("b"), total(0) { };
		
		const KeyType& GetOwner() const { return owner; }
		const IBAN& GetIBAN() const { return iban; }
		const BIC& GetBIC() const { return bic; }
		const Quantity<Sum>& GetTotal() const { return total; }
		const auto& GetDirection() const { return direction.Id(); }		
		
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
				out<<"\tDate: "<<Bank::Get<DateTimes::Date>(*(*it))<<"\tSum: "<<std::setprecision(2)<<std::fixed<<Bank::Get<Quantity<Sum>>(**it)<<std::endl;
				out<<"\t"<<"\t"<<Bank::Get<Entry>(**it)<<std::endl;
			}

			out<<std::endl;
		}
		
		ResultContainer GetCause(std::string name = "")
		{
			ResultContainer result;
			for(auto it = this->transactions.Begin(); it != this->transactions.End(); ++it)
			{
				if(String_::Contains(Bank::Get<Entry>(*(*it)).Value, name))
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
		using AccountEndpointType = AccountEndpoint<typename T::AccountType>;
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
		
	template<typename T, typename TT>
	std::ostream& operator<<(std::ostream& out, const Transfer<T,TT>& s)
	{
		out<<std::setw(30)<<std::left<<s.GetOwner()<<std::setw(60)<<s.GetTransaction()<<std::setw(20)<<std::right<<s.GetDate()<<std::setw(10)<<std::setprecision(2)<<std::fixed<<s.GetQuantity()<<"\n";
		return out<<std::setw(30)<<std::left<<s.GetIBAN()<<std::setw(60)<<s.GetBIC()<<std::setw(20);		
	}
}


#endif
