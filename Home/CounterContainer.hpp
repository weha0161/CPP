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
#include "../Tuple/Tuple.hpp"

#ifndef COUNTERCONTAINER_HPP
#define COUNTERCONTAINER_HPP

//---------------------------------------------------------------------------------------------------CounterContainer----------------------------------------------------------------------------------------

template<typename Head, typename... Tail>
class CounterContainer
{
public:
	using CounterTypes = Tuple<Head,Tail...>;
private:
	CounterTypes counters;
public:

	void Display(std::ostream& os) const
	{
		os<<this->counters;
	}
	
	void Write(const std::string sourcePath = ".")
	{
		TupleWrite(counters);
	}
	
	void Read(const std::string sourcePath = ".")
	{
		TupleRead(counters);
	}
	
	template<unsigned N>
	auto Get() { return GetNum<N>(counters); }

	template<typename T>
	auto Get() { return GetType<T>(counters); }
	
	CounterContainer():counters(CounterTypes()) { };
};

template<typename Head, typename... Tail>
std::ostream& operator<<(std::ostream& strm, const CounterContainer<Head,Tail...> c)
{
	c.Display(strm);
	return strm;
}

#endif
