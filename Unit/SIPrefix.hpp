#include <memory>
#include <ratio>
#include "../Fraction/Fraction.h"

#ifndef SIPREFIX_HPP
#define SIPREFIX_HPP

template<int Ex = 0>
struct SIPrefix
{
// 	using Type = Fraction<N,D>::Type;
	static const std::string Sign;
	static constexpr int Exponent = Ex;
	static constexpr double Factor = 1;
};

// template<> std::string SIPrefix<1000,1>::Name = "Kilo"
// using Kilo = SIPrefix<1000,1>::Type;

struct Kilo: public SIPrefix<3>
{
    inline static const std::string Sign = "Kilo";
	static constexpr double Factor = 1000;
};


#endif
