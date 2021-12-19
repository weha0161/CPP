#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Unit/Unit.h"
#include "../CSV/CSV.hpp"
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


template<typename T,  int No = 0, typename U = typename T::Unit>
struct CounterConfiguration
{
	static const uint Number = No;
	using Type = T;
	using Unit = U;
};

template<typename U, typename T = double, typename DateT = Date>
struct Reading
{
	using Unit = U;
	const DateT Date;
	const T Value;
	
	Reading(T val, DateT d): Date(d), Value(val){}
};

template<typename ConfigT>
class Counter
{
	using Config = ConfigT;
	using ReadinType = Reading<typename Config::Unit>;
	using ReadingContainerType = std::vector<ReadinType>;
	
private:
	const std::string name;
	std::unique_ptr<ReadingContainerType> readings = std::unique_ptr<ReadingContainerType>(new ReadingContainerType());
	
public:	
	static const uint Number = Config::Number;
	using Type = Config::Type;
	using Unit = Config::Unit;
	
	Counter(const std::string s):name(s){} 
	
	const std::string Name() { return this->name; }
	
	static void Display(std::ostream& out) /*const*/
	{
		out<<Unit::UnitSign()<<"\t"<<Number<<std::endl;
// 		for(auto p : this->transfers)
// 		{
// 			out<<"\tdate: "<<p.getdate()<<"\tsum: "<<std::setprecision(2)<<std::fixed<<p.getquantity()<<std::endl;
// 		}

		out<<std::endl;
	}
// 	Read
// 	Write
};

#endif
