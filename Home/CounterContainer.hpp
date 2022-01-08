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
#include "../Logger/Logger.hpp"
#include "../Typelist/Typelist.h"

#ifndef COUNTERCONTAINER_HPP
#define COUNTERCONTAINER_HPP

//---------------------------------------------------------------------------------------------------CounterContainer----------------------------------------------------------------------------------------

template<typename List>
class CounterContainer{};

template<typename Head>
class CounterContainer<Typelist<Head>>
{
public:
	using Type = Head;
	using CounterTypes = Typelist<Head>;
	using ContainerType = CounterContainer<Typelist<Head>>;
protected:
	CounterContainer() { Logger::Log<Info>()<<"CounterContainer created."<<std::endl; };
public:
// 	CounterContainer(const CounterContainer& c) { *this = CounterContainer::Instance(); };

	static std::ostream& Display(std::ostream& os) 
	{
		return Type::Display(os);
	}
	
	void Write(const std::string sourcePath = ".")
	{
		Type::Write(sourcePath);
	}
	
	void Read(const std::string sourcePath = ".")
	{
		Type::Read();
	}
	
	template<unsigned N>
	auto Get() { return At<CounterTypes,N>::Type; }

	template<typename T>
	auto Get() { return GetType<CounterTypes,T>; }
	
	static CounterContainer& Instance()
	{
		static CounterContainer instance;
		return instance;
	}	
};

template<typename Head, typename... Tail>
class CounterContainer<Typelist<Head,Tail...>>: public CounterContainer<Typelist<Tail...>>
{
public:
	using Type = Head;
	using CounterTypes = Typelist<Head,Tail...>;
	using ContainerType = CounterContainer<Typelist<Head,Tail...>>;
protected:
	CounterContainer() { Logger::Log<Info>()<<"CounterContainer created."<<std::endl; };
public:
// 	CounterContainer(const CounterContainer& c) { *this = CounterContainer::Instance(); };

	static std::ostream& Display(std::ostream& os) 
	{
		return CounterContainer<Typelist<Tail...>>::Display(Type::Display(os));		
	}
	
	void Write(const std::string sourcePath = ".")
	{
		Type::Write(sourcePath);
		CounterContainer<Typelist<Tail...>>::Write(sourcePath);		
	}
	
	void Read(const std::string sourcePath = ".")
	{
		Type::Read();
		CounterContainer<Typelist<Tail...>>::Read();		
	}
	
	template<unsigned N>
	auto Get() { return At<CounterTypes,N>::Type; }

	template<typename T>
	auto Get() { return GetType<CounterTypes,T>; }
	
	static CounterContainer& Instance()
	{
		static CounterContainer instance;
		return instance;
	}	
};

template<typename Head, typename... Tail>
std::ostream& operator<<(std::ostream& strm, const CounterContainer<Head,Tail...> c)
{
	return c.Display(strm);
}

using CG1 = Counter<GasConfiguration>;
using CE1 = Counter<EnBWEnergyConfiguration>;
using CWA = Counter<AllWaterConfiguration>;
using CWO = Counter<OWaterConfiguration>;
using CWOut = Counter<OutWaterConfiguration>;

using CBHW = Counter<Bottom_HWaterConfiguration>;
using CBCW = Counter<Bottom_CWaterConfiguration>;
using CMCW = Counter<Middle_CWaterConfiguration>;
using CMHW = Counter<Middle_HWaterConfiguration>;
using CTCW = Counter<Top_CWaterConfiguration>;
using CTHW = Counter<Top_HWaterConfiguration>;

using CounterConatinerType = CounterContainer<Typelist<CE1,CG1,CWA,CWO,CWOut, CBCW,CBHW, CMCW,CMHW,CTCW,CTHW>>::ContainerType;

// static CounterConatinerType& CtrContainer =	 CounterConatinerType:.Instance;

#endif
