#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Unit/Unit.h"
#include "../Quantity/Quantity.h"
#include "Parser.hpp"
#include "CounterConfiguration.hpp"
#include "StageQuantities.hpp"
#include "StageConfiguration.hpp"

#ifndef UTILITIESSTATEMENT_HPP
#define UTILITIESSTATEMENT_HPP

template<typename ConfigT>
class UtilitiesStatement
{
public:
	using Type = UtilitiesStatement<ConfigT>;
	using Configuration = ConfigT;
	
	inline static const char* Name = Configuration::Name;
		
	static UtilitiesStatement& Instance()
	{
		static UtilitiesStatement instance = UtilitiesStatement();
		return instance;
	}
	
	
	
	std::ostream& Display(std::ostream& os) 
	{
		return os;
	}
	
private:
	
	
	UtilitiesStatement(){ Logger::Log()<<"CTOR: "<<"UtilitiesStatement"<<std::endl;}
	
	~UtilitiesStatement()	{ Logger::Log()<<"Destructor"<<std::endl; }
	UtilitiesStatement& operator=(const UtilitiesStatement&) = delete;
	UtilitiesStatement(const UtilitiesStatement& c) = delete;
};

template<typename C, typename S = T::char_<'\t'>>
std::ostream& operator<<(std::ostream& strm, const UtilitiesStatement<C> c)
{
	return c.Display(strm);
}


#endif
