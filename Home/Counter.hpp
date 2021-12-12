#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Unit/Unit.h"
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/vector.hpp>
#include <map>
#include <chrono>
#include <ctime>

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

struct Water: public CounterType<Water,Mass>
{ 
	inline static std::string CounterType<Water, Mass>::Name = "Water"; 
};

struct Gas: public CounterType<Gas,Length>
{ 
	inline static std::string CounterType<Gas, Length>::Name = "Gas"; 
};


template<typename T, typename U = typename T::Unit, int No = 0>
struct CounterConfiguration
{
	static const uint Number = No;
	using Type = T;
	using Unit = U;
};

template<typename Config>
class Counter
{
	const std::string name;
public:
		
	const uint Number = Config::Number;
	using Type = Config::Type;
	using Unit = Config::Unit;
	
	Counter(const std::string s):name(s){} 
	
	const std::string Name() { return this->name; }
};

#endif
