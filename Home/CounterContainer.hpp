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
template<typename List>
class CounterContainer	{};

template<typename Head>
class CounterContainer<Typelist<Head>>
{
public:
	using Type = Head;
	
	void Display(std::ostream& os)
	{
		Head::Display(os);
	}
	
	CounterContainer()	{ }
};

template<typename Head, typename... Tail>
class CounterContainer<Typelist<Head,Tail...>>: public CounterContainer<Typelist<Tail...>>
{
public:
	using Type = Head;

	void Display(std::ostream& os)
	{
		Head::Display(os);
		
		CounterContainer<Typelist<Tail...>>::Display(os);
	}
	
	CounterContainer() { };
};



#endif
