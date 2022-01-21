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

struct BuildingInsurance{ constexpr static const char* Name = "BuildingInsurance"; };

template<typename T, typename Q>
struct CalculatorConfiguration
{
	static const char* AccountKey;
	using Type = T;
	using QuantityType = Q;
	constexpr static const char* Name = T::Name; 
};

template<> const char* CalculatorConfiguration<BuildingInsurance,Persons>::AccountKey = "TEST";
// template<> const char* CSVValue<IndividualUnit, Scalar, unsigned>::Key = "IndividualUnit";

using BuildingInsuranceConfig = CalculatorConfiguration<BuildingInsurance, Persons>;
#endif
