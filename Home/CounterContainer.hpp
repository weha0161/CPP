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

	void Display(std::ostream& os)
	{
// 		auto cw3 = GetNum<3>(counters);
// 		cw3.Display(os);
		os<<this->counters;
	}
	
	template<unsigned N>
	auto Get() { return GetNum<N>(counters); }

	template<typename T>
	auto Get() { return GetType<T>(counters); }
	
	CounterContainer():counters(CounterTypes()) { };
};

#endif
