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
#include "Stage.hpp"
#include "../Logger/Logger.hpp"
#include "../Typelist/Typelist.h"

#ifndef STAGECONTAINER_HPP
#define STAGECONTAINER_HPP

//---------------------------------------------------------------------------------------------------StageContainer----------------------------------------------------------------------------------------

using StagesMap = std::map<std::string, StageMap>;

template<typename List>
class StageContainer{};

template<typename Head>
class StageContainer<Typelist<Head>>
{
public:
	using Type = Head;
	using StageTypes = Typelist<Head>;
	using ContainerType = StageContainer<Typelist<Head>>;
protected:
	StageContainer() 
	{ 
		Head::Instance(StageMap()); 
		Logger::Log<Info>()<<"StageContainer created."<<std::endl; 
		
	};
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
	auto Get() { return At<StageTypes,N>::Type; }

// 	template<typename T>
// 	auto Get() { return GetType<StageTypes,T>; }
// 	
	static StageContainer& Instance()
	{
		static StageContainer instance;
		return instance;
	}	
};

template<typename Head, typename... Tail>
class StageContainer<Typelist<Head,Tail...>>: public StageContainer<Typelist<Tail...>>
{
public:
	using Type = Head;
	using StageTypes = Typelist<Head,Tail...>;
	using ContainerType = StageContainer<Typelist<Head,Tail...>>;
protected:
	StageContainer() 
	{ 
		Head::Instance(StageMap()); 
		Logger::Log<Info>()<<"StageContainer created."<<std::endl; 
		
	};
public:
	static std::ostream& Display(std::ostream& os) 
	{
		return StageContainer<Typelist<Tail...>>::Display(Type::Display(os));		
	}
	
	void Write(const std::string sourcePath = ".")
	{
		Type::Write(sourcePath);
		StageContainer<Typelist<Tail...>>::Write(sourcePath);		
	}
	
	void Read(const std::string sourcePath = ".")
	{
		Type::Read();
		StageContainer<Typelist<Tail...>>::Read();		
	}
	
	template<unsigned N>
	auto Get() { return At<StageTypes,N>::Type; }
/*
	template<typename T>
	auto Get() { return GetType<StageTypes,T>; }
	*/
	static StageContainer& Instance()
	{
		static StageContainer instance;
		return instance;
	}	
};

template<typename Head, typename... Tail>
std::ostream& operator<<(std::ostream& strm, const StageContainer<Head,Tail...> c)
{
	return c.Display(strm);
}

using StageContainerType = StageContainer<Typelist<Top,Middle,Bottom>>::ContainerType;

#endif
