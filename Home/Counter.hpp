#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Unit/Unit.h"
#include "../CSV/CSV.hpp"
#include "../File/Info.hpp"
#include "../File/Node.hpp"
#include "../CSV/CSV.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "Parser.hpp"
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/vector.hpp>
#include <map>
#include <chrono>
#include <ctime>
#include <memory>

#ifndef COUNTER_H
#define COUNTER_H

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


template<typename MeterType,  int No = 0, typename U = typename MeterType::Unit>
struct CounterConfiguration
{
	static const uint Number = No;
	inline static const std::string CounterName = String_::FromInt(No) + "_" + MeterType::Name;
	inline static const std::string DestinataionPath = "//home//markus//Downloads//";
	inline static const std::string AdditionalInformation = "-";
	using MeterT = MeterType;
	using Unit = U;
};

template<typename U, typename ValT = double, typename DateT = Date>
struct Reading
{
	using Unit = U;
	const DateT Date;
	const ValT Value;
	
	template<typename Separator = T::char_<';'>>
	void Display(std::ostream& out) const
	{
		out<<Date<<(char)Separator::Value<<Value<<std::endl;
	}
	
	Reading(ValT val, DateT d): Date(d), Value(val){}
};

template<typename C,typename T = double, typename DateT = Date>
std::ostream& operator<<(std::ostream& strm, const Reading<C,T,DateT> c)
{
	c.Display(strm);
	return strm;
}

template<typename ConfigT>
class Counter
{
private:
	using Config = ConfigT;
	using MeterType = Config::MeterT;
	using ReadingType = Reading<typename Config::Unit>;
	using ReadingContainerType = std::vector<ReadingType>;
	inline static const std::string DestinationPath = Config::DestinataionPath;
	inline static const std::string Name = Config::CounterName;
	
	std::unique_ptr<ReadingContainerType> readings = std::unique_ptr<ReadingContainerType>(new ReadingContainerType());
	std::unique_ptr<FS::FileInfo> fileInfo = std::unique_ptr<FS::FileInfo>(new FS::FileInfo(std::filesystem::path(DestinationPath + Name)));
	std::unique_ptr<FS::CSV> csv = std::unique_ptr<FS::CSV>(new FS::CSV(this->fileInfo.get()));
	
public:	
	using Type = Config::MeterT;
	using Unit = Config::Unit;
	static const uint Number = Config::Number;
	
	Counter(){}
	Counter(const Counter& c){}
	
	void Display(std::ostream& out) const
	{
		out<<Name<<std::endl;
		out<<Number<<std::endl;
		out<<Type::Name<<std::endl;
		out<<Unit::SiUnit()<<std::endl;
		out<<std::endl;
		
		this->readings->push_back(ReadingType(9.0, Date("30.09.2021")));
		for(auto it = this->readings->cbegin(); it != this->readings->cend(); ++it)
			(*it).Display(out);
	}
	
	std::string GetName()
	{
		return Name;
	}
	
	void Read(){}
	void Write(const std::string sourcePath = ".")
	{
		csv->Write(*this);
	}
};

template<typename C>
std::ostream& operator<<(std::ostream& strm, const Counter<C> c)
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
using Bottom_HWaterConfiguration = CounterConfiguration<Water,15018324, Volume>;
using Bottom_KWaterConfiguration = CounterConfiguration<Water,15007241, Volume>;
using Middle_KWaterConfiguration = CounterConfiguration<Water,23267492, Volume>;
using Middle_HWaterConfiguration = CounterConfiguration<Water,14524889, Volume>;
using Top_KWaterConfiguration = CounterConfiguration<Water,25489823, Volume>;
using Top_HWaterConfiguration = CounterConfiguration<Water,25470737, Volume>;

#endif
