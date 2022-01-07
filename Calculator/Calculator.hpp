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
	const TReading& FirstReading;
	const TReading& SecondReading;
	const TQuantity& Result;
	
	CalculatorResult(TReading r1, TReading r2, TQuantity q): FirstReading(r1), SecondReading(r2), Result(q) {};	
};

template<typename TC, typename Q = typename TC::QuantityType, typename T = typename  TC::ReadingType>
struct Difference
{ 
	static CalculatorResult<T,Q> Calculate(const T& t1, const T& t2) 
	{ 
// 		Logger::Log()<<"t1: "<<t1.QuantityValue.Value<<std::endl;
// 		Logger::Log()<<"t2: "<<t2.QuantityValue.Value<<std::endl;
		std::cout<<"t2: "<<std::endl;
		return CalculatorResult(t1, t2, t1.QuantityValue - t2.QuantityValue); 
	}
};

#endif
