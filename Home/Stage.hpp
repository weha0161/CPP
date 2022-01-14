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

#ifndef STAGE_HPP
#define STAGE_HPP

template<typename Derived,int N, unsigned A, unsigned R>
struct StageConfiguration
{
	using Type = Derived;
	static constexpr int Number = N;
	static constexpr unsigned Area = A;
	static constexpr unsigned Rooms = R;
	static const char* Name;
};

struct TopConfiguration:StageConfiguration<TopConfiguration,2,58,5> { };
struct MiddleConfiguration:StageConfiguration<MiddleConfiguration,1,101,7> { };
struct BottomConfiguration:StageConfiguration<BottomConfiguration,1,101,7> { };

template<> const char* StageConfiguration<TopConfiguration, TopConfiguration::Number, TopConfiguration::Area, TopConfiguration::Rooms>::Name = "Top";
template<> const char* StageConfiguration<MiddleConfiguration,MiddleConfiguration::Number, MiddleConfiguration::Area, MiddleConfiguration::Rooms>::Name = "Middle";
template<> const char* StageConfiguration<BottomConfiguration, BottomConfiguration::Number, BottomConfiguration::Area, BottomConfiguration::Rooms>::Name = "BottomConfiguration";

template<typename D, typename U, typename T = double>
class CSVValue: public Element
{
	using Derived = D;
	using Unit = U;
public:
	CSVValue(std::string s = "0.0"): Element(s), quantity(this->to(s)) {};
	static const char* Key;
	Element* DoCreate() { return this; };
private:
	Quantity<U> quantity;
	T val;
	String_::To<T> to;
};

class ApartmentArea: public CSVValue<ApartmentArea, Area>
{
public:
	ApartmentArea(std::string s = "0.0"): CSVValue(s) {};
};


class Rooms: public CSVValue<Rooms, Scalar, unsigned>
{
public:
	Rooms(std::string s = "0"): CSVValue(s) {};
};	


template<> const char* CSVValue<ApartmentArea, Area>::Key = "Area";
template<> const char* CSVValue<Rooms, Area>::Key = "Area";


template<typename ConfigT>
class Stage
{
public:
	using Type = Stage<ConfigT>;
	using Configuration = ConfigT;
	static constexpr int Number = Configuration::Number;
	static constexpr unsigned Area = Configuration::Area;
	static constexpr unsigned Rooms = Configuration::Rooms;
	inline static const char* Name = Configuration::Name;
		
private:
	
};

template<typename C, typename S = T::char_<'\t'>>
std::ostream& operator<<(std::ostream& strm, const Stage<C> c)
{
	return c.Display(strm);
}

using Top = Stage<TopConfiguration>;
using Middle = Stage<MiddleConfiguration>;
using Bottom = Stage<BottomConfiguration>;

#endif
