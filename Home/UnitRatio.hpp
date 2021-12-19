#include <memory>
#include <ratio>
#include "../Fraction/Fraction.h"

#ifndef UNITRATIO_HPP
#define UNITRATIO_HPP

template<int N,int D>
struct UnitRatio
{
	using Type = Fraction<N,D>::Type;
	static const std::string Sign;
};

// template<> std::string UnitRatio<1000,1>::Name = "Kilo"
// using Kilo = UnitRatio<1000,1>::Type;

struct Kilo: public UnitRatio<1000,1>
{
    inline static const std::string Sign = "Kilo";
};

#endif
