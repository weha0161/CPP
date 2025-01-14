#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Unit/Unit.h"
#include "../Quantity/Quantity.h"
#include "../CSV/CSV.hpp"
#include "../File/Info.hpp"
#include "../File/Node.hpp"
#include "../Unit/SIPrefix.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../Common/DateTimes.hpp"
#include "Parser.hpp"
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/vector.hpp>
#include <map>
#include <chrono>
#include <ctime>
#include <memory>

#ifndef COUNTERCONFIGURATION_HPP
#define COUNTERCONFIGURATION_HPP

template<typename Derived, typename U>
struct CounterType
{
	using Unit = U;
	using Type = Derived;
	static std::string Name;
};

struct Energy: public CounterType<Energy,Current>
{ 
	inline static std::string CounterType<Energy, Current>::Name = "Energy"; 
};

struct Water: public CounterType<Water,Volume>
{ 
	inline static std::string CounterType<Water, Volume>::Name = "Water"; 
};

struct Gas: public CounterType<Gas,Volume>
{ 
	inline static std::string CounterType<Gas, Volume>::Name = "Gas"; 
};


struct None
{
	inline static const std::string Value = "";
};
struct Hot
{
	inline static const std::string Value = "_Hot";
};
struct Cold
{
	inline static const std::string Value = "_Cold";
};

template<typename T>
struct AdditionalInformation
{
	inline static const std::string Value = T::Value;
};

template<typename MeterType,  int No = 0, typename U = typename MeterType::Unit, typename A = AdditionalInformation<None>>
struct CounterConfiguration
{
	static const uint Number = No;
	inline static const std::string AdditionalInformation = A::Value;
	inline static const std::string CounterName = String_::FromInt(No) + "_" + MeterType::Name + AdditionalInformation;
	inline static const std::string DestinationPath = "//home//markus//Downloads//";
	using MeterT = MeterType;
	using Unit = U;
};

template<typename U, typename Pre = SIPrefix<0>, typename Q = Quantity<U,Pre,double>, typename DateT = DateTimes::Date>
struct Reading
{
	using Unit = U;
	using QuantityType = Q;
	using DateType = DateT;
	const DateType Date;
	const QuantityType QuantityValue;
	
	template<typename Separator = T::char_<';'>>
	void Display(std::ostream& out) const
	{
		out<<Date<<Separator::Value<<QuantityValue.Value()<<Separator::Value<<QuantityType::UnitPrefix::Sign<<U::Sign()<<std::endl;
	}
	
	Reading(QuantityType val, DateType d): Date(d), QuantityValue(val)	{}
};

template<typename C,typename T = double, typename DateT = Date>
std::ostream& operator<<(std::ostream& strm, const Reading<C,T,DateT> c)
{
	c.Display(strm);
	return strm;
}

using GasConfiguration = CounterConfiguration<Gas,1202757, Volume>;
using EnBWEnergyConfiguration = CounterConfiguration<Energy,21740069, Work>;
using VattenfallEnergyConfiguration = CounterConfiguration<Energy,11144078, Work>;
using AllWaterConfiguration = CounterConfiguration<Water,18061860, Volume>;
using OWaterConfiguration = CounterConfiguration<Water,279638, Volume>;
using OutWaterConfiguration = CounterConfiguration<Water,458711, Volume>;
using Bottom_HWaterConfiguration = CounterConfiguration<Water,15018324, Volume, AdditionalInformation<Hot>>;
using Bottom_CWaterConfiguration = CounterConfiguration<Water,15007241, Volume, AdditionalInformation<Cold>>;
using Middle_CWaterConfiguration = CounterConfiguration<Water,23267492, Volume, AdditionalInformation<Cold>>;
using Middle_HWaterConfiguration = CounterConfiguration<Water,14524889, Volume, AdditionalInformation<Hot>>;
using Top_CWaterConfiguration = CounterConfiguration<Water,25489823, Volume, AdditionalInformation<Cold>>;
using Top_HWaterConfiguration = CounterConfiguration<Water,25470737, Volume, AdditionalInformation<Hot>>;

#endif
