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
template<typename... Types>
class CounterContainer;

template<typename Head, typename... Tail>
class CounterContainer<Head,Tail...>//: public CounterContainer<Tail...>
{
private:
	Head head;
	CounterContainer<Tail...> tail;
public:
	using Type = Head;

	static void Display(std::ostream& os)
	{
		Head::Display(os);
		
		CounterContainer<Tail...>::Display(os);
	}
	
	CounterContainer<Tail...>& GetTail() 
	{
		return tail;
	}
	
	Head& GetHead() 
	{
		return head;
	}
	
	CounterContainer() { };
};

template<>
class CounterContainer<>
{
public:	
	static void Display(std::ostream& os)
	{
// 		Head::Display(os);
	}
// 	CounterContainer() { };
};
// template<typename Head>
// class CounterContainer<Head>
// {
// public:	
// 	void Display(std::ostream& os)
// 	{
// 		Head::Display(os);
// 	}
// 	CounterContainer() { };
// };

template<unsigned N>
struct ConatainerGet
{
	template<typename Head, typename... Tail>
	static auto apply(CounterContainer<Head,Tail...> const& c)
	{
		return ConatainerGet<N-1>::apply(c.GetTail());
	}
};

template<>
struct ConatainerGet<0>
{
	template<typename Head, typename... Tail>
	static auto apply(CounterContainer<Head,Tail...> const& c)
	{
		return c.GetHead();
	}
};

template<unsigned N, typename... Types>
auto get(CounterContainer<Types...> c)
{
	return ConatainerGet<N>::apply(c);
};

#endif
