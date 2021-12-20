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
// #include "../Typelist/Typelist.h"	

#ifndef TUPLE_HPP
#define TUPLE_HPP

//---------------------------------------------------------------------------------------------------Tuple----------------------------------------------------------------------------------------
template<typename... Types>
class Tuple;

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

template<typename... Types>
struct FrontTuple;

template<typename Head, typename... Tail>
struct FrontTuple<Tuple<Head, Tail...>> 
{
	using Type = Head;
};

template<typename... Types>
struct PopFrontTuple;

// remove front element:
template<typename Head, typename... Tail>
struct PopFrontTuple<Tuple<Head, Tail...>> 
{
	using Type = Tuple<Tail...>;
};

template<unsigned N>
struct TupleGetNum
{
	template<typename Head, typename... Tail>
	static auto apply(Tuple<Head,Tail...> const& t)
	{
		return TupleGetNum<N-1>::apply(t.GetTail());
	}
};

template<>
struct TupleGetNum<0>
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
	return TupleGetNum<N>::apply(t);
};

//-------------------------------------------------------
/*
// template<typename T>
template<typename T1, typename T2>
struct TupleGet
{
	template<typename Head, typename... Tail>
	static auto apply(Tuple<Head,Tail...> const& t)
	{		
		return TupleGet<T1,PopFrontTuple<Tuple<Head>>::Type>::apply(t.GetTail());
	}
};

// template<typename T, typename Head, typename... Tail>
template<typename T>
struct TupleGet<T,T>
{
	template<typename Head, typename... Tail>
	static auto apply(Tuple<Head,Tail...> const& t)
	{
		return t.GetHead();
	}
};*/
// template<typename T1, typename T2>
template<typename T1, typename T2>
struct TupleGet
{
	template<typename Head, typename... Tail>
	static auto apply(Tuple<Head,Tail...> const& t)
	{	
		return TupleGet<T1,typename Tuple<Tail...>::Type>::apply(t.GetTail());
	}
};

// template<typename T>
template<typename T>
struct TupleGet<T,T>
{
	template<typename Head, typename... Tail>
	static auto apply(Tuple<Head,Tail...> const& t)
	{
		return t.GetHead();
	}
};

template<typename T, typename... Types>
auto Get(Tuple<Types...> t)
{
	return TupleGet<T,typename Tuple<Types...>::Type>::apply(t);
};

//-------------------------------------------------------

#endif
