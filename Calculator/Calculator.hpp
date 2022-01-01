#include <memory>
#include <ratio>
#include "../Fraction/Fraction.h"
#include "../Home/Counter.hpp"
#include "../Logger/Logger.hpp"

#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

template<typename T>
struct Difference
{ 
	static T Calculate(T t1, T t2) 
	{ 
		return t1 - t2; 
	}
};

#endif
