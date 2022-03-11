#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Unit/Unit.h"
#include "../Quantity/Quantity.h"
#include "../Calculator/Calculator.hpp"
#include "Parser.hpp"
#include "CounterConfiguration.hpp"
#include "StageQuantities.hpp"
#include "StageConfiguration.hpp"

#ifndef UTILITIESSTATEMENT_HPP
#define UTILITIESSTATEMENT_HPP
using YearType = uint;

template<typename ConfigT>
class UtilitiesStatement
{
public:
	using Type = UtilitiesStatement<ConfigT>;
	using Configuration = ConfigT;
	using Stage = Configuration::Stage;
	using All = Configuration::All;
	
	inline static const char* Name = Configuration::Name;
		
	static UtilitiesStatement& Instance()
	{
		static UtilitiesStatement instance = UtilitiesStatement();
		return instance;
	}
	
	UtilitiesStatement(){ Logger::Log()<<"CTOR: "<<"UtilitiesStatement"<<std::endl;}	
	UtilitiesStatement(YearType y): year(y) { Logger::Log()<<"CTOR: "<<"UtilitiesStatement"<<y<<std::endl;}	
	~UtilitiesStatement()	{ Logger::Log()<<"Destructor"<<std::endl; }
	
	void Calculate() 
	{
		Logger::Log()<<"US: "<<Stage::Number<<std::endl;
		Calculator::Stage<BuildingInsurance>::template Calculate<Stage,All>();
		Calculator::Stage<WasteFees>::template Calculate<Stage,All>();
		Calculator::Stage<ChimneySweeper>::template Calculate<Stage,All>();
		Calculator::Stage<PropertyTax>::template Calculate<Stage,All>();
		Calculator::Stage<Sewage>::template Calculate<Stage,All>();
	}
	
	std::ostream& Display(std::ostream& os) 
	{
		return os;
	}
	
private:
	YearType year;
	
};

template<typename C, typename S = T::char_<'\t'>>
std::ostream& operator<<(std::ostream& strm, const UtilitiesStatement<C> c)
{
	return c.Display(strm);
}


#endif
