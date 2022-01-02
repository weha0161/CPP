#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Unit/Unit.h"
#include "../Quantity/Quantity.h"
#include "../CSV/CSV.hpp"
#include "../File/Info.hpp"
#include "../File/Node.hpp"
#include "../Unit/SIPrefix.hpp"
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

template<typename U, typename Pre = SIPrefix<0>, typename Q = Quantity<U,Pre,double>, typename DateT = Date>
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

template<typename T>
struct Deleter {
  void operator()(T* t) {	Logger::Log()<<"unique_ptr DELETED!!!!!!!!!!!!"<<std::endl; }
};

template<typename ConfigT>
class Counter
{
public:
	using Config = ConfigT;
	using MeterType = Config::MeterT;
	using ReadingType = Reading<typename Config::Unit>;
	using QuantityType = ReadingType::QuantityType;
	using DateType = ReadingType::DateType;
	using ReadingContainerType = std::vector<ReadingType>;
	using CIterator = std::vector<ReadingType>::const_iterator;
private:
	inline static const std::string DestinationPath = Config::DestinationPath;
	inline static const std::string Name = Config::CounterName;
	
	static std::map<std::string, std::string> createHeader()
	{
		std::map<std::string, std::string> m;
		m["CounterName"] = Name;
		m["Number"] =  std::to_string(Config::Number),
		m["Type"] = MeterType::Name,
		m["SiUnit"] =  Config::Unit::SiUnit(),
		m["Unit"] =  Config::Unit::Sign();
        return m;
	}
	
	ReadingType CreateReading(std::vector<std::string> values)
	{
		auto date = Date(values.at(0));
		auto value = std::stod(values.at(1));
		
		return ReadingType(QuantityType(value), DateType(date));
	}
	
	inline static const std::map<std::string, std::string> Header = createHeader();
	
	inline static std::unique_ptr<ReadingContainerType, Deleter<ReadingContainerType>> readings = std::unique_ptr<ReadingContainerType, Deleter<ReadingContainerType>>(new ReadingContainerType(),Deleter<ReadingContainerType>());
	std::unique_ptr<FS::FileInfo> fileInfo = std::unique_ptr<FS::FileInfo>(new FS::FileInfo(std::filesystem::path(DestinationPath + Name + FS::CSV::Extension)));
	std::unique_ptr<FS::CSV> csv = std::unique_ptr<FS::CSV>(new FS::CSV(this->fileInfo.get()));
	
	Counter(){ Logger::Log()<<"Ctor: "<<MeterType::Name<<"_"<<Config::Number<<std::endl; };
// 	~Counter(){}
public:
	Counter& operator=(const Counter&) { return Counter::Instance(); };
	static Counter& Instance()
	{
		static Counter instance;
		return instance;
	}
	
	Counter(const Counter& c) { *this = Counter::Instance(); };
	
	using Type = MeterType;
	using Unit = Config::Unit;
	inline static const uint Number = Config::Number;
	
	
	template<typename Separator = T::char_<'\t'>>
	void DisplayHeader(std::ostream& out) const
	{
		for (auto& it : Header)
			out<<it.first<<Separator::Value<<it.second<<std::endl;
	}
	
	template<typename Separator = T::char_<'\t'>>
	void Display(std::ostream& out) const
	{
		DisplayHeader(out);
		out<<std::endl;
		
		this->readings->push_back(ReadingType(QuantityType(45.0), Date("30.09.2021")));
		this->readings->push_back(ReadingType(QuantityType(40.0), Date("30.09.2021")));
		this->readings->push_back(ReadingType(QuantityType(35.0), Date("30.09.2021")));
		this->readings->push_back(ReadingType(QuantityType(30.0), Date("30.09.2021")));
		for(auto it = this->readings->cbegin(); it != this->readings->cend(); ++it)
			(*it).Display(out);
		
		Logger::Log()<<this-readings->size()<<std::endl;
	}
	
	std::string GetName() const
	{
		return Name;
	}
	
	void Read()
	{
		
		Logger::Log()<<"Read counter in path: "<<this->fileInfo->Path()<<std::endl;
		auto values = csv->Read();
		
		for(int i = Header.size() + 1; i < values.size(); ++i)
		{
			auto reading = this->CreateReading(values.at(i));
			this->readings->push_back(reading);
		}
	}
	
	CIterator Begin() const { return this->readings->cbegin(); }
	CIterator End() const { return this->readings->cend(); }
	
	template<template<typename> class TCalc, typename Calc = TCalc<QuantityType>>
	void Calculate()
	{
		for(auto it = this->readings->cbegin(); it != this->readings->cend(); ++it)
		{
			auto v = Calc::Calculate(this->Begin()->QuantityValue, (it+1)->QuantityValue);
			Logger::Log()<<"V RESULT"<<v.Value()<<std::endl;
		}		
	};

	
	void Write(const std::string sourcePath = ".")
	{
		csv->Write(*this);
	}
};

template<typename C, typename S = T::char_<'\t'>>
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
using Bottom_HWaterConfiguration = CounterConfiguration<Water,15018324, Volume, AdditionalInformation<Hot>>;
using Bottom_CWaterConfiguration = CounterConfiguration<Water,15007241, Volume, AdditionalInformation<Cold>>;
using Middle_CWaterConfiguration = CounterConfiguration<Water,23267492, Volume, AdditionalInformation<Cold>>;
using Middle_HWaterConfiguration = CounterConfiguration<Water,14524889, Volume, AdditionalInformation<Hot>>;
using Top_CWaterConfiguration = CounterConfiguration<Water,25489823, Volume, AdditionalInformation<Cold>>;
using Top_HWaterConfiguration = CounterConfiguration<Water,25470737, Volume, AdditionalInformation<Hot>>;

#endif
