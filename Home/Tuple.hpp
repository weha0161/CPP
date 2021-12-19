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

//---------------------------------------------------------------------------------------------------Tuple----------------------------------------------------------------------------------------
template<typename... Types>
class Tuple	;

template<typename Head, typename... Tail>
class Tuple<Head,Tail...>
{
private:
	Head head;
	Tuple<Tail...> tail;
public:
	using Type = Head;
		
	Tuple(): head(Head()) {
	};
	Tuple(Head const & head, Tuple<Tail...> const& tail): head(head), tail(tail) { };
	
	Head& GetHead() {return head;}
	Head const& GetHead() const {return head;}
	
	Tuple<Tail...>& GetTail() {return tail;}
	Tuple<Tail...> const& GetTail() const {return tail;}
	
};

template<>
class Tuple<>
{
};

template<unsigned N>
struct TupleGet
{
	template<typename Head, typename... Tail>
	static auto apply(Tuple<Head,Tail...> const& t)
	{
		return TupleGet<N-1>::apply(t.GetTail());
	}
};

template<>
struct TupleGet<0>
{
	template<typename Head, typename... Tail>
	static auto apply(Tuple<Head,Tail...> const& t)
	{
		return t.GetHead();
	}
};

template<unsigned N, typename... Types>
auto Get(Tuple<Types...> t)
{
	return TupleGet<N>::apply(t);
};

#endif
