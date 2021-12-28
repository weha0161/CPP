#include <algorithm>
#include <functional>
#include <iostream>
#include <ctime>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <filesystem>
#include "../Logger/Logger.hpp"
#include "../Wrapper/Wrapper.hpp"

#ifndef MATH_HPP
#define MATH_HPP

//-------------------------------------------------------------Get by Num-----------------------------------------------------------

template<int N, int Ex>
struct Calculate
{
	static double constexpr Result = N * Calculate<N,Ex-1>::Result;
};

template<int N>
struct Calculate<N,0>
{
	static double constexpr Result = 1;
};


template <int N, int Ex, bool isNegative = false> 
struct GetPow 
{
	static double constexpr Result = Calculate<N,Ex>::Result;
};

template <int N, int Ex> 
struct GetPow<N, Ex,true> 
{
	static double constexpr Result = 1 / (Calculate<N,-Ex>::Result);
};

template <int N, int Ex> 
struct Pow 
{
	static double constexpr Result = GetPow<N,Ex, Ex < 0>::Result;
};

// template<int N>
// struct Pow<N,0,false>
// {
// 	static double constexpr Result = 1;
// };


//-------------------------------------------------------

#endif
