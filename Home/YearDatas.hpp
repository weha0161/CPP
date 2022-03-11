#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include <memory>
// #include "UtilitiesStatement.hpp"

#ifndef YEARDATAS_HPP
#define YEARDATAS_HPP


template<typename>
struct UtilitiesStatement;

using YearType = uint;

template<typename T>
struct YearDatas
{
	using UtilitiesStatementPtrType = std::shared_ptr<UtilitiesStatement<T>>;
	
	YearDatas(YearType y): year(y) 
	{ 		
		Logger::Log()<<"CTOR: "<<"YearDatas"<<y<<std::endl;
	}	
	~YearDatas(){ Logger::Log()<<"Destructor"<<std::endl; }
	
	std::ostream& Display(std::ostream& os) 
	{
		return os;
	}
	
private:
	YearType year;
	UtilitiesStatementPtrType ut = std::make_shared<UtilitiesStatement<T>>(); 	
};

#endif
