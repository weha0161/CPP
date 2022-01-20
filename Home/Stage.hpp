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

using StageMap = std::map<std::string, std::string>;

template<typename Derived,int N, unsigned A, unsigned R, unsigned I = 1>
struct StageConfiguration
{
	using Type = Derived;
	static constexpr int Units = I;
	static constexpr int Number = N;
	static constexpr unsigned Area = A;
	static constexpr unsigned Rooms = R;
	static const char* Name;
};

struct TopConfiguration:StageConfiguration<TopConfiguration,2,58,5,2> { };
struct MiddleConfiguration:StageConfiguration<MiddleConfiguration,1,101,7> { };
struct BottomConfiguration:StageConfiguration<BottomConfiguration,0,101,7> { };

template<> const char* StageConfiguration<TopConfiguration, TopConfiguration::Number, TopConfiguration::Area, TopConfiguration::Rooms, TopConfiguration::Units>::Name = "Top";
template<> const char* StageConfiguration<MiddleConfiguration,MiddleConfiguration::Number, MiddleConfiguration::Area, MiddleConfiguration::Rooms>::Name = "Middle";
template<> const char* StageConfiguration<BottomConfiguration, BottomConfiguration::Number, BottomConfiguration::Area, BottomConfiguration::Rooms>::Name = "Bottom";

template<typename D, typename U, typename T = double>
class CSVValue: public Element
{
	using Derived = D;
public:
	using Unit = U;
	CSVValue(std::string s = "0.0"): Element(s), quantity(this->to(s)) {};
	CSVValue(T t): Element(std::to_string(t)), quantity(t) {};
	const Quantity<U>& Get() { return this->quantity; }
	const T& GetValue() { return this->val; }
	static const char* Key;
	Element* DoCreate() { return this; };
private:
	Quantity<U> quantity;
	T val;
	String_::To<T> to;
};

class IndividualUnit: public CSVValue<IndividualUnit, Area, unsigned>
{
public:
	IndividualUnit(unsigned a): CSVValue(a) {};
};

class ApartmentArea: public CSVValue<ApartmentArea, Area, unsigned>
{
public:
	ApartmentArea(unsigned a): CSVValue(a) {};
};

class Rooms: public CSVValue<Rooms, Scalar, unsigned>
{
public:
	Rooms(unsigned r): CSVValue(r) {};
};	

class Persons: public CSVValue<Persons, Scalar, unsigned>
{
public:
	Persons(const std::string& s = "0"): CSVValue(s) {};
};	

class Advance: public CSVValue<Advance, Sum>
{
public:
	Advance(std::string s = "0"): CSVValue(s) {};
};

class IncidentalHeatingCosts: public CSVValue<IncidentalHeatingCosts, Sum>
{
public:
	IncidentalHeatingCosts(std::string s = "0"): CSVValue(s) {};
};	

class GarageRental: public CSVValue<GarageRental, Sum>
{
public:
	GarageRental(std::string s = "0"): CSVValue(s) {};
};	

class MonthlyRent: public CSVValue<MonthlyRent, Sum>
{
public:
	MonthlyRent(std::string s = "0"): CSVValue(s) {};
};	


template<> const char* CSVValue<IndividualUnit, Scalar, unsigned>::Key = "IndividualUnit";
template<> const char* CSVValue<ApartmentArea, Area, unsigned>::Key = "Area";
template<> const char* CSVValue<Rooms, Area>::Key = "Rooms";
template<> const char* CSVValue<Persons, Scalar, unsigned>::Key = "Persons";
template<> const char* CSVValue<Advance, Sum>::Key = "Advance";
template<> const char* CSVValue<IncidentalHeatingCosts, Sum>::Key = "IncidentalHeatingCosts";
template<> const char* CSVValue<MonthlyRent, Sum>::Key = "MonthlyRent";
template<> const char* CSVValue<GarageRental, Sum>::Key = "GarageRental";


template<typename ConfigT>
class Stage
{
public:
	using Type = Stage<ConfigT>;
	using Configuration = ConfigT;
	static constexpr int Number = Configuration::Number;
	inline static const char* Name = Configuration::Name;
	
	const Quantity<Area> AreaQuantity() { return this->area.Get(); }
	const Quantity<Area> RoomsQuantity() { return this->rooms.Get(); }
	const Quantity<Scalar> UnitsQuantity() { return this->individualUnit.Get(); }
	const Quantity<Scalar> PersonsQuantity() { return this->persons.Get(); }
	const Quantity<Sum> AdvanceQuantity() { return this->advance.Get(); }
	const Quantity<Sum> IncidentalHeatingCostsQuantity() { return this->incidentalHeatingCosts.Get(); }
	const Quantity<Sum> MonthlyRentQuantity() { return this->monthlyRent.Get(); }
	const Quantity<Sum> GarageRentalQuantity() { return this->garageRental.Get(); }
	
	static Stage& Instance()
	{
		static Stage instance = Stage(Map);
		return instance;
	}
	
	static void Set(const StageMap& m)
	{
		Map = m;
	}
	
	std::ostream& Display(std::ostream& os) 
	{
		os<<"Name: "<<Type::Name<<"\tUnits:"<<individualUnit.Get()<<"\tRooms:"<<rooms.Get()<<"\tArea: "<<area.Get()<<"\tPersons: "<<persons.Get();
		return os<<"MonthlyRent: "<<monthlyRent.Get()<<"\tAdvance: "<<advance.Get()<<"\tGarage: "<<garageRental.Get()<<"\tIncidentalHeatingCosts: "<<incidentalHeatingCosts.Get()<<std::endl;
	}
	
private:
	inline static StageMap Map = StageMap();
	static constexpr unsigned AreaValue = Configuration::Area;
	static constexpr unsigned RoomsValue = Configuration::Rooms;
	static constexpr unsigned UnitsValue = Configuration::Units;
	
	ApartmentArea area = ApartmentArea(AreaValue);
	Rooms rooms = Rooms(RoomsValue);
	IndividualUnit individualUnit = IndividualUnit(UnitsValue);
	Persons persons;
	Advance advance;
	IncidentalHeatingCosts incidentalHeatingCosts;
	GarageRental garageRental;
	MonthlyRent monthlyRent;
	
	Stage(){ Logger::Log()<<"CTOR: "<<Number<<std::endl;}
	Stage(const StageMap& m): 
		persons(m.at(Persons::Key)),
		advance(m.at(Advance::Key)),
		monthlyRent(m.at(MonthlyRent::Key)),
		incidentalHeatingCosts(m.at(IncidentalHeatingCosts::Key)),
		garageRental(m.at(GarageRental::Key))
	{ 
// 		for(auto kv : m)
// 			Logger::Log()<<kv.first<<": "<<kv.second<<std::endl;
// 		Logger::Log()<<"CTOR MAP: "<<Number<<std::endl;
	}
	
	static Stage INSTANCE;
	~Stage()	{ Logger::Log()<<"Destructor"<<std::endl; }
	Stage& operator=(const Stage&) = delete;
	Stage(const Stage& c) = delete;
};

template<typename C, typename S = T::char_<'\t'>>
std::ostream& operator<<(std::ostream& strm, const Stage<C> c)
{
	return c.Display(strm);
}


template<typename T, typename Q>
struct Get{};

template<typename T>
struct Get<T, ApartmentArea>
{
	static Quantity<ApartmentArea::Unit> Value() { return T::Instance().AreaQuantity(); }
};

template<typename T>
struct Get<T, Persons>
{
	static Quantity<Persons::Unit> Value() { return T::Instance().PersonsQuantity(); }
};

using Top = Stage<TopConfiguration>;
using Middle = Stage<MiddleConfiguration>;
using Bottom = Stage<BottomConfiguration>;

#endif
