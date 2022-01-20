#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Unit/Unit.h"
#include "../Quantity/Quantity.h"
#include "../CSV/CSV.hpp"
#include "../File/Info.hpp"
#include "../File/Node.hpp"
#include "../Unit/SIPrefix.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "Parser.hpp"
#include "CounterConfiguration.hpp"
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/vector.hpp>
#include <map>
#include <chrono>
#include <ctime>
#include <memory>

#ifndef HOUSE_HPP
#define HOUSE_HPP

struct HouseConfiguration
{
	
};

template<typename ConfigT = HouseConfiguration>
class House
{
public:
	using Type = House<ConfigT>;
	using Configuration = ConfigT;
	inline static const char* Name = "House";
	
	static House& Instance()
	{
		static House instance;
		return instance;
	}
	
	std::ostream& Display(std::ostream& os) 
	{
		return os;
	}
	
private:
	
	House(){ Logger::Log()<<"CTOR: "<<Name<<std::endl;}
	
	~House()	{ Logger::Log()<<"Destructor"<<std::endl; }
	House& operator=(const House&) = delete;
	House(const House& c) = delete;
};

template<typename C, typename S = T::char_<'\t'>>
std::ostream& operator<<(std::ostream& strm, const House<C>& c)
{
	return c.Display(strm);
}

#endif
