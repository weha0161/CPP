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
#include "../Wrapper/Wrapper.hpp"
// #include "../Typelist/Typelist.h"	

#ifndef MATH_HPP
#define MATH_HPP

//-------------------------------------------------------------Get by Num-----------------------------------------------------------

template<int N, int Ex>
struct Pow
{
	static int constexpr Result = N * Pow<N,Ex-1>::Result;
};

template<int N>
struct Pow<N,0>
{
	static int constexpr Result = 1;
};

//-------------------------------------------------------

#endif
