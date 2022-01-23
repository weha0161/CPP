#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Unit/Unit.h"
#include "../Quantity/Quantity.h"
#include "../CSV/CSV.hpp"
#include "../File/Info.hpp"
#include "../File/Node.hpp"
#include "../Unit/SIPrefix.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../Home/StageQuantities.hpp"
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/vector.hpp>
#include <map>
#include <chrono>
#include <ctime>
#include <memory>

#ifndef CALCULATORCONFIGURATION_HPP
#define CALCULATORCONFIGURATION_HPP


template<typename Derived, typename Q>
struct CalculatorConfiguration
{
	inline static const Key AccountKey = Key(Derived::KeyString);
	using Type = Derived;
	using QuantityType = Q;
	constexpr static const char* Name = Derived::Name; 
};

struct BuildingInsurance: CalculatorConfiguration<BuildingInsurance, IndividualUnit> 
{ 
	constexpr static const char* Name = "BuildingInsurance"; 
	constexpr static const char* KeyString = "SV Gebaeudeversicherung"; 
};

// template<> const char* CalculatorConfiguration<BuildingInsurance,Persons>::AccountKey = "SV Gebaeudeversicherung";
// template<> const char* CSVValue<IndividualUnit, Scalar, unsigned>::Key = "IndividualUnit";
#endif
