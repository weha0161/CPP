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

//-------------------------------------------------------------------------------------------------CalculatorOperation----------------------------------------------------------
template<typename R, typename Q, typename S = T::char_<'\t'>>
std::ostream& operator<<(std::ostream& strm, const CalculatorResult<R,Q> cr)
{
	return cr.Display(strm);
}

template<class Derived>
struct CalculatorOperation
{
	static const char* Name; 
	static const char* Sign; 
};

struct Difference: CalculatorOperation<Difference>
{ 
	using Type = Difference;
	
	template<typename T, typename Q>
	static CalculatorResult<T,Q> Calculate(const T& t1, const T& t2) {	return CalculatorResult(t1, t2, t1.QuantityValue - t2.QuantityValue); }
};

template<> const char* CalculatorOperation<Difference>::Name = "Difference";
template<> const char* CalculatorOperation<Difference>::Sign = "-";

//-------------------------------------------------------------------------------------------------Calculator----------------------------------------------------------

template<class TCounter, typename TCalc>
struct Calculator
{
	using ReadingType =  TCounter::ReadingType;
	using QuantityType = TCounter::QuantityType;
	static void Calculate()
	{
		for(auto it = TCounter::Begin(); (it + 1) != TCounter::End(); ++it)
		{
			auto v = TCalc::template Calculate<ReadingType,QuantityType>(*it, *(it+1));
			Logger::Log()<<v<<std::endl;
			Logger::Log()<<TCalc::Name<<TCalc::Sign<<std::endl;
		}		
	}
};

#endif
