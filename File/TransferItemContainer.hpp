#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <chrono>
#include <tuple>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <filesystem>
#include "AccountTransfer.hpp"
#include "../CSV/CSV.hpp"
#include "../Unit/Unit.h"
#include "../Quantity/Quantity.h"
#include "../Common/DateTimes.hpp"
#include "../Logger/Logger.hpp"
#include "../Typelist/Typelist.h"

#ifndef TRANSFERCONTAINER_HPP
#define TRANSFERCONTAINER_HPP

//---------------------------------------------------------------------------------------------------CounterContainer----------------------------------------------------------------------------------------

template<typename KeyIndexContainerType, typename List>
class TransferItemContainer{};

template<typename KeyIndexContainerType,typename Head>
class TransferItemContainer<KeyIndexContainerType,Typelist<Head>>
{
public:
	using Type = Head;
	using CounterTypes = Typelist<Head>;
	using ContainerType = TransferItemContainer<KeyIndexContainerType,Typelist<Head>>;
	using TupleType = std::tuple<Type>;
protected:
	TransferItemContainer() { Logger::Log<Info>()<<"TransferItemContainer created."<<std::endl; };

	auto createTransfer(const std::string& s) { return TupleType(Type(Type::Identifier));}
	
	template<typename T, typename Cont = T::KeyIndexContainerType::ContainerType>
	auto Create(const std::string sourcePath, Cont ret)
	{
		Logger::Log()<<"CREATE: "<<Head::Identifier<<std::endl;
		ret->push_back(typename T::KeyIndexContainerType::KeyIndexType(Type::Identifier));
		return ret;		
	}
public:
	static std::ostream& Display(std::ostream& os) 
	{
		return Type::Display(os);
	}
	
	void Read(const std::string& sourcePath = ".")
	{
		Logger::Log()<<Type::Identifier<<std::endl;
	}
	

	template<unsigned N>
	auto Get() { return At<CounterTypes,N>::Type; }

	static TransferItemContainer& Instance()
	{
		static TransferItemContainer instance;
		return instance;
	}	
};

template<typename KeyIndexContainerType,typename Head, typename... Tail>
class TransferItemContainer<KeyIndexContainerType,Typelist<Head,Tail...>>: public TransferItemContainer<KeyIndexContainerType,Typelist<Tail...>>
{
public:
	using Type = Head;
	using CounterTypes = Typelist<Head,Tail...>;
	using ContainerType = TransferItemContainer<KeyIndexContainerType,Typelist<Head,Tail...>>;
	using Base = TransferItemContainer<KeyIndexContainerType, Typelist<Tail...>>;
	using TupleType = std::tuple<Type>;
protected:
	TransferItemContainer() { Logger::Log<Info>()<<"TransferItemContainer created."<<std::endl; };
	
	auto createTransfer(const std::string& s) 
	{
		auto bT = Base::createTransfer(s); 
		auto result = std::tuple_cat(TupleType(Type(Type::Identifier)), bT);
		return result;
	} 
	
	template<typename T, typename Cont = T::KeyIndexContainerType::ContainerType>
	auto Create(const std::string& sourcePath, Cont ret)
	{
		Logger::Log()<<"CREATE: "<<Head::Identifier<<std::endl;
		ret->push_back(typename T::KeyIndexContainerType::KeyIndexType(Type::Identifier));
		return Base::template Create<T>(sourcePath, std::move(ret));	
	}
public:
	static std::ostream& Display(std::ostream& os) 
	{
		return Base::Display(Type::Display(os));		
	}
		
	void Read(const std::string& sourcePath = ".")
	{
		Logger::Log()<<Type::Identifier<<std::endl;
		Base::Read();		
	}
	
	auto CreateTransfer(const std::string& s)
	{
		auto t = this->createTransfer(s);
		return Bank::AccountTransfer("","",0.0,"");
	}
	
	template<typename T>
	auto Create(const std::string& sourcePath = ".")
	{
		auto ret = std::make_unique<typename T::KeyIndexContainerType::ContainerType>();
		
		//~ auto t = this->createTransfer(sourcePath);
		//~ auto ib = std::get<IBAN>(t);
		//~ Logger::Log<Error>()<<"CREATE Tuple: "<<ib.Value<<"\t"<<std::tuple_size<decltype(t)>::value<<std::endl;
		
		ret->push_back(typename T::KeyIndexContainerType::KeyIndexType(Type::Identifier));
		return Base::template Create<T>(sourcePath, std::move(ret));		
	}
	
	template<unsigned N>
	auto Get() { return At<CounterTypes,N>::Type; }

	static TransferItemContainer& Instance()
	{
		static TransferItemContainer instance;
		return instance;
	}	
};

template<typename Head, typename... Tail>
std::ostream& operator<<(std::ostream& strm, const TransferItemContainer<Head,Tail...> c)
{
	return c.Display(strm);
}


//Buchungstag;Valuta;Textschlüssel;Primanota;Zahlungsempfänger;ZahlungsempfängerKto;ZahlungsempfängerIBAN;ZahlungsempfängerBLZ;ZahlungsempfängerBIC;Vorgang/Verwendungszweck;Kundenreferenz;Währung;Umsatz;Soll/Haben
//~ using TransferItemContainerType = TransferItemContainer<Typelist<IBAN,BIC,DateTimes::Date,IBAN, Quantity<Sum>, Bank::Transfer<Bank::In>, Bank::Transfer<Bank::Out>>>::ContainerType;

#endif
