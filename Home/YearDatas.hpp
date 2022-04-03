#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Common/DateTimes.hpp"
#include <memory>

#ifndef YEARDATAS_HPP
#define YEARDATAS_HPP

template<typename>
struct UtilitiesStatement;

using YearType = DateTimes::Year;

template<typename T>
struct YearDatas
{
	using UtilitiesStatementPtrType = std::shared_ptr<UtilitiesStatement<T>>;
	
	YearDatas(YearType y): year(y) 
	{ 		
		Logger::Log()<<"CTOR: "<<"YearDatas"<<y.Value()<<std::endl;
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
