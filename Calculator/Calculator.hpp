#include <memory>
#include <ratio>
#include "CalculatorConfiguration.hpp"
#include "../Fraction/Fraction.h"
#include "../Home/Counter.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../Logger/Logger.hpp"
#include "../File/Raiba.hpp"
#include "../File/Account.hpp"

#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

namespace Calculator
{
	template<typename TReading, typename TQuantity>
	struct Result			
	{
		const TReading FirstReading;
		const TReading SecondReading;
		const TQuantity Value;
		
		template<typename Separator = T::char_<'\t'>>
		std::ostream& Display(std::ostream& out) const
		{
			return out<<FirstReading<<Separator::Value<<SecondReading<<Separator::Value<<Value<<std::endl;
		}
		
		Result(TReading r1, TReading r2, TQuantity q): FirstReading(r1), SecondReading(r2), Value(q) {};	
	};

	template<typename R, typename Q, typename S = T::char_<'\t'>>
	std::ostream& operator<<(std::ostream& strm, const Result<R,Q> cr)
	{
		return cr.Display(strm);
	}

	//-------------------------------------------------------------------------------------------------CalculatorOperation----------------------------------------------------------
	template<class Derived>
	struct CalculatorOperation
	{
		static const char* Name; 
		static const char* Sign; 
	};
	
	struct Ratio: CalculatorOperation<Ratio>
	{ 
		using Type = Ratio;
		
		template<typename T, typename Q>
		static Result<T,Q> Calculate(const T& nom, const T& denom, const Q& sum) {	return Result(nom, denom, nom / denom * sum); }
	};
	
	template<> const char* CalculatorOperation<Ratio>::Name = "Ratio";
	template<> const char* CalculatorOperation<Ratio>::Sign = "%";

	struct Difference: CalculatorOperation<Difference>
	{ 
		using Type = Difference;
		
		template<typename T, typename Q>
		static Result<T,Q> Calculate(const T& t1, const T& t2) {	return Result(t1, t2, t1.QuantityValue - t2.QuantityValue); }
	};

	template<> const char* CalculatorOperation<Difference>::Name = "Difference";
	template<> const char* CalculatorOperation<Difference>::Sign = "-";

	struct Addition: CalculatorOperation<Addition>
	{ 
		using Type = Addition;
		
		template<typename T, typename Q>
		static Result<T,Q> Calculate(const T& t1, const T& t2) {	return Result(t1, t2, t1.QuantityValue + t2.QuantityValue); }
	};
	
	template<> const char* CalculatorOperation<Addition>::Name = "Addition";
	template<> const char* CalculatorOperation<Addition>::Sign = "+";

	//-------------------------------------------------------------------------------------------------Calculator----------------------------------------------------------
	template<class Config>
	struct Stage
	{
		
		template<typename Stage, typename AllStages>
		static void Calculate()
		{
			auto total = AllStages::Instance().template GetTotal<typename Config::QuantityType>();
			
			auto g = Get<Bank::Raiba<0>, Bank::Out>();
			auto gr = g(Config::AccountKey);
			gr.Display(std::cout);
			
			if(T::IsSame<Config,BuildingInsurance>::Value)
			{
				if(T::IsSame<Stage,Top>::Value)
					Ratio::Calculate(IndividualUnit(2).Get(), IndividualUnit(4).Get(), gr.GetTotal());
				else
					Ratio::Calculate(IndividualUnit(1).Get(), IndividualUnit(4).Get(), gr.GetTotal());
				
				return;
			}
			
			auto result = Ratio::Calculate(GetQuantity<Stage, typename Config::QuantityType>::Value(), total, gr.GetTotal());
			
			std::cout<<"CALC: "<<result<<std::endl;
			std::cout<<"CALC: "<<Config::Name<<std::endl;
			std::cout<<"CALC: "<<Config::AccountKey<<std::endl;
		}
	};
	
	
	template<class TCounter, typename TCalc>
	struct Counter
	{
		using ReadingType =  TCounter::ReadingType;
		using QuantityType = TCounter::QuantityType;
		static std::vector<Result<ReadingType,QuantityType>> Calculate()
		{
			auto result = std::vector<Result<ReadingType,QuantityType>>(); 
			for(auto it = TCounter::Begin(); (it + 1) != TCounter::End(); ++it)
			{
				auto cr = TCalc::template Calculate<ReadingType,QuantityType>(*it, *(it+1));
				result.push_back(cr);
			}
			
			return result;
		}
	};	
	
	template<class TCounter, typename TCalc>
	struct StageWater
	{
		using ReadingType =  TCounter::ReadingType;
		using QuantityType = TCounter::QuantityType;
		static std::vector<Result<ReadingType,QuantityType>> Calculate()
		{
			auto result = std::vector<Result<ReadingType,QuantityType>>(); 
			for(auto it = TCounter::Begin(); (it + 1) != TCounter::End(); ++it)
			{
				auto cr = TCalc::template Calculate<ReadingType,QuantityType>(*it, *(it+1));
				result.push_back(cr);
			}
			
			return result;
		}
	};	
}


#endif
