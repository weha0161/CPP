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
#include "CounterConfiguration.hpp"
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/vector.hpp>
#include <map>
#include <chrono>
#include <ctime>
#include <memory>

#ifndef COUNTER_H
#define COUNTER_H

template<typename T>
struct DebugDeleter {
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
	using Type = MeterType;
	using Unit = Config::Unit;
	using CIterator = std::vector<ReadingType>::const_iterator;
	inline static const uint Number = Config::Number;

	Counter& operator=(const Counter&) { return Counter::Instance(); };
	static Counter& Instance()
	{
		static Counter instance;
		return instance;
	}
	
	Counter(const Counter& c) 
	{ 
		*this = Counter::Instance(); 
	};	
	
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
		
		for(auto it = this->readings->cbegin(); it != this->readings->cend(); ++it)
			(*it).Display(out);
		
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
	
	void Write(const std::string sourcePath = ".")
	{
		csv->Write(*this);
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
	
	
	~Counter()
	{
		Logger::Log(this->Begin(), this->End());
	}
	
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
	inline static std::unique_ptr<ReadingContainerType, DebugDeleter<ReadingContainerType>> readings = std::unique_ptr<ReadingContainerType, DebugDeleter<ReadingContainerType>>(new ReadingContainerType(),DebugDeleter<ReadingContainerType>());
	
	inline static std::unique_ptr<FS::FileInfo> fileInfo = std::unique_ptr<FS::FileInfo>(new FS::FileInfo(std::filesystem::path(DestinationPath + Name)));
	inline static std::unique_ptr<FS::CSV> csv = std::unique_ptr<FS::CSV>(new FS::CSV(fileInfo.get()));
	
	Counter()
	{ 
		Logger::Log()<<"Ctor: "<<this->Name<<MeterType::Name<<"_"<<Config::Number<<std::endl; 
		this->Read();
	};
};

template<typename C, typename S = T::char_<'\t'>>
std::ostream& operator<<(std::ostream& strm, const Counter<C> c)
{
	c.Display(strm);
	return strm;
}

#endif
