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

#ifndef HOUSE_HPP
#define HOUSE_HPP

//---------------------------------------------------------------------------------------------------House----------------------------------------------------------------------------------------

template<typename List>
class House{};

template<typename Head>
class House<Typelist<Head>>
{
public:
	using Type = Head;
	using CounterTypes = Typelist<Head>;
	using ContainerType = House<Typelist<Head>>;
protected:
	House() { Logger::Log<Info>()<<"House created."<<std::endl; };
public:
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

// 	template<typename T>
// 	auto Get() { return GetType<CounterTypes,T>; }
// 	
	static House& Instance()
	{
		static House instance;
		return instance;
	}	
};

template<typename Head, typename... Tail>
class House<Typelist<Head,Tail...>>: public House<Typelist<Tail...>>
{
public:
	using Type = Head;
	using CounterTypes = Typelist<Head,Tail...>;
	using ContainerType = House<Typelist<Head,Tail...>>;
protected:
	House() { Logger::Log<Info>()<<"House created."<<std::endl; };
public:
	static std::ostream& Display(std::ostream& os) 
	{
		return House<Typelist<Tail...>>::Display(Type::Display(os));		
	}
	
	void Write(const std::string sourcePath = ".")
	{
		Type::Write(sourcePath);
		House<Typelist<Tail...>>::Write(sourcePath);		
	}
	
	void Read(const std::string sourcePath = ".")
	{
		Type::Read();
		House<Typelist<Tail...>>::Read();		
	}
	
	template<unsigned N>
	auto Get() { return At<CounterTypes,N>::Type; }
/*
	template<typename T>
	auto Get() { return GetType<CounterTypes,T>; }
	*/
	static House& Instance()
	{
		static House instance;
		return instance;
	}	
};

template<typename Head, typename... Tail>
std::ostream& operator<<(std::ostream& strm, const House<Head,Tail...> c)
{
	return c.Display(strm);
}

using HouseType = House<Typelist<CE1,CG1,CWA,CWO,CWOut, CBCW,CBHW, CMCW,CMHW,CTCW,CTHW>>::ContainerType;

#endif
