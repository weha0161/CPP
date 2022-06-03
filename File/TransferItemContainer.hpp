#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
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

template<typename List>
class TransferItemContainer{};

template<typename Head>
class TransferItemContainer<Typelist<Head>>
{
public:
	using Type = Head;
	using CounterTypes = Typelist<Head>;
	using ContainerType = TransferItemContainer<Typelist<Head>>;
protected:
	TransferItemContainer() { Logger::Log<Info>()<<"TransferItemContainer created."<<std::endl; };

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
		
	void Read(const std::string sourcePath = ".")
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

template<typename Head, typename... Tail>
class TransferItemContainer<Typelist<Head,Tail...>>: public TransferItemContainer<Typelist<Tail...>>
{
public:
	using Type = Head;
	using CounterTypes = Typelist<Head,Tail...>;
	using ContainerType = TransferItemContainer<Typelist<Head,Tail...>>;
	using Base = TransferItemContainer<Typelist<Tail...>>;
protected:
	TransferItemContainer() { Logger::Log<Info>()<<"TransferItemContainer created."<<std::endl; };
	
	template<typename T, typename Cont = T::KeyIndexContainerType::ContainerType>
	auto Create(const std::string sourcePath, Cont ret)
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
		
	void Read(const std::string sourcePath = ".")
	{
		Logger::Log()<<Type::Identifier<<std::endl;
		Base::Read();		
	}
	
	template<typename T>
	auto Create(const std::string sourcePath = ".")
	{
		Logger::Log()<<"CREATE: "<<Head::Identifier<<std::endl;
		Logger::Log()<<"CREATE: "<<Base::Type::Identifier<<std::endl;
		auto ret = std::make_unique<typename T::KeyIndexContainerType::ContainerType>();
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
using TransferItemContainerType = TransferItemContainer<Typelist<IBAN,BIC,DateTimes::Date,IBAN, Quantity<Sum>, Bank::Transfer<Bank::In>, Bank::Transfer<Bank::Out>>>::ContainerType;

#endif
