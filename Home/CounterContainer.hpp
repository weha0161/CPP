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

template<typename Head, typename... Tail>
class CounterContainer
{
public:
	using CounterTypes = Typelist<Head,Tail...>;
private:
// 	CounterTypes counters;
	CounterContainer() { Logger::Log<Info>()<<"CounterContainer created."<<std::endl; };
public:
// 	CounterContainer(const CounterContainer& c) { *this = CounterContainer::Instance(); };

	void Display(std::ostream& os) const
	{
// 		os<<this->counters;
	}
	
	void Write(const std::string sourcePath = ".")
	{
	}
	
	void Read(const std::string sourcePath = ".")
	{
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
	c.Display(strm);
	return strm;
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

using CounterConatinerType = CounterContainer<CE1,CG1,CWA,CWO,CWOut, CBCW,CBHW, CMCW,CMHW,CTCW,CTHW>;

// static CounterConatinerType& CtrContainer =	 CounterConatinerType:.Instance;

#endif
