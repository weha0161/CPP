#include <memory>
#include <ratio>
#include "../Fraction/Fraction.h"
#include "../Math/Math.hpp"

#ifndef SIPREFIX_HPP
#define SIPREFIX_HPP

template<int Ex = 0>
struct SIPrefix
{
	static constexpr int Exponent = Ex;
	static constexpr int BigDistance = Ex % 3;
	using Type = SIPrefix<Exponent>;
	static inline const std::string Sign;
	static inline const std::string Name;
// 	static inline const std::string Sign = "";
// 	static inline const std::string Name = "None";
	static constexpr double Factor = Math::Pow<10,Ex>::Result;
	
	using Next = SIPrefix<Exponent+1>;
	using Prev = SIPrefix<Exponent-1>;
};


// template<>
// struct SIPrefix<2> 
// {	
// 	inline static const std::string Sign = "h"; 
// 	inline static const std::string Name = "Hekto"; 
// };

struct Zero: public SIPrefix<0> 
{	
	inline static const std::string Sign = ""; 
	inline static const std::string Name = ""; 
};

struct Kilo: public SIPrefix<3> 
{	
	inline static const std::string Sign = "k"; 
	inline static const std::string Name = "Kilo"; 
};

struct Hekto: public SIPrefix<2> 
{	
	inline static const std::string Sign = "h"; 
	inline static const std::string Name = "Hekto"; 
};

struct Deka: public SIPrefix<1> 
{	
	inline static const std::string Sign = "da"; 
	inline static const std::string Name = "Deka";
};

#endif
