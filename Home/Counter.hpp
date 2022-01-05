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
		DisplayHeader<Separator>(out);		
		for(auto it = this->readings->cbegin(); it != this->readings->cend(); ++it)
			(*it).Display(out);
		
	}
	
	std::string GetName() const
	{
		return Name;
	}
	
	void Read()
	{
		auto values = csv->Read();
		Logger::Log()<<values.size()<<" "<<Header.size()<<"\tRead counter in path: "<<this->fileInfo->Path()<<FS::CSV::Extension<<std::endl;
		
		for(int i = Header.size(); i < values.size(); ++i)
		{
			auto reading = this->CreateReading(values.at(i).cbegin(), values.at(i).cend());
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
	
	template<typename Iterator>
	ReadingType CreateReading(Iterator cbegin, Iterator cend)
	{
		if(cbegin != cend)
		{
			try 
			{
				auto date = Date(*cbegin);
				auto value = std::stod(*(++cbegin));
				
				return ReadingType(QuantityType(value), DateType(date));
			} 
			catch (const std::exception& e) 
			{
				Logger::Log<Error>()<<"Error: CreateReading"<<e.what()<<std::endl;
			}		
		}
		else
			Logger::Log<Error>()<<"Error: CreateReading-> Not enough values"<<std::endl;
			
				
		return ReadingType(QuantityType(0.0), DateType(Date("01.01.2000")));
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
