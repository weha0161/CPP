#include <memory>
#include <ratio>
#include "../Fraction/Fraction.h"
#include "../Home/Counter.hpp"
#include "../Logger/Logger.hpp"

#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

template<typename TCalc, typename TCounter>
class Calculator
{
	using Iterator = TCounter::ReadingContainerType::const_iterator;
	const Iterator& begin;
	const Iterator& end;
	
public:
// template<typename TCalc, typename Iterator /*= std::const_iterator*/>
	Calculator(const Iterator& b, const Iterator& e): begin(b), end(e){}
	
	void Calculate()
	{
		for(auto it = this->begin; it != this->end; ++it)
			Logger::Log()<<*it<<std::endl;

	};
};


#endif
