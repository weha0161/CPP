#include <memory>
#include <ratio>
#include "../Fraction/Fraction.h"
#include "../Home/Counter.hpp"
#include "../Logger/Logger.hpp"

#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

template<typename TReading, typename TQuantity>
struct CalculatorResult			
{
	const TReading FirstReading;
	const TReading SecondReading;
	const TQuantity Result;
	
	template<typename Separator = T::char_<'\t'>>
	std::ostream& Display(std::ostream& out) const
	{
		return out<<FirstReading<<Separator::Value<<SecondReading<<Separator::Value<<Result<<std::endl;
	}
	
	CalculatorResult(TReading r1, TReading r2, TQuantity q): FirstReading(r1), SecondReading(r2), Result(q) {};	
};

template<typename R, typename Q, typename S = T::char_<'\t'>>
std::ostream& operator<<(std::ostream& strm, const CalculatorResult<R,Q> cr)
{
	return cr.Display(strm);
}

template<template<typename> class Derived>
// template<typename Derived>
struct CalculatorBase
{
	static const char* Name; 
	static const char* Sign; 
};

template<typename TC, typename Q = typename TC::QuantityType, typename T = typename  TC::ReadingType>
struct Difference: CalculatorBase<Difference>
{ 
	using Type = Difference<TC>;
	static CalculatorResult<T,Q> Calculate(const T& t1, const T& t2) {	return CalculatorResult(t1, t2, t1.QuantityValue - t2.QuantityValue); }
};

template<> const char* CalculatorBase<Difference>::Name = "Difference";
template<> const char* CalculatorBase<Difference>::Sign = "-";

#endif
