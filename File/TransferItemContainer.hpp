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
	using InputIterator = std::vector<std::string>::const_iterator;
	using ContainerType = TransferItemContainer<KeyIndexContainerType,Typelist<Head>>;
	using KeyIndexContainerPtrType = std::shared_ptr<KeyIndexContainerType>;
	using TupleType = std::tuple<Type>;
protected:
	TransferItemContainer() { Logger::Log<Info>()<<"TransferItemContainer created."<<std::endl; };
	KeyIndexContainerPtrType keyIndices;
	auto createTransfer(InputIterator begin, InputIterator end) 
	{ 
		Logger::Log()<<Head::Identifier<<"_"<<(*(this->keyIndices))[Head::Identifier]<<" => "<<*(begin + (*(this->keyIndices))[Head::Identifier])<<std::endl;
		return TupleType(Type(Type::Identifier));
	}
	
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
	
	void setKeyIndexContainer(KeyIndexContainerPtrType ptr){ this->keyIndices = ptr; }

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
	
	auto createTransfer(Base::InputIterator begin, Base::InputIterator end) 
	{
		Logger::Log()<<Head::Identifier<<"_"<<(*(this->keyIndices))[Head::Identifier]<<" => "<<*(begin + (*(this->keyIndices))[Head::Identifier])<<std::endl;
		auto bT = Base::createTransfer(begin,end); 
		auto result = std::tuple_cat(TupleType(Type(*(begin + (*(this->keyIndices))[Head::Identifier]))), bT);
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
	
	template<typename TransferType>
	auto CreateTransfer(Base::InputIterator begin, Base::InputIterator end)
	{
		Logger::Log()<<Head::Identifier<<"_"<<(*(this->keyIndices))[Head::Identifier]<<" => "<<*(begin + (*(this->keyIndices))[Head::Identifier])<<std::endl;
		auto t = Base::createTransfer(begin,end);
		//~ return std::make_shared<TransferType>("","",0.0,"");
		return 0;
	}
	
	template<typename T>
	auto Create(const std::string& sourcePath = ".")
	{
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
//~ using TransferItemContainerType = TransferItemContainer<Typelist<IBAN,BIC,DateTimes::Date,IBAN, Quantity<Sum>, Bank::Transfer<Bank::In>, Bank::Transfer<Bank::Out>>>::ContainerType;

#endif
