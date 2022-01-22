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
#include "StageQuantities.hpp"
#include "StageConfiguration.hpp"
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/vector.hpp>
#include <map>
#include <chrono>
#include <ctime>
#include <memory>

#ifndef STAGE_HPP
#define STAGE_HPP

using StageMap = std::map<std::string, std::string>;

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
struct GetQuantity{};

template<typename T>
struct GetQuantity<T, ApartmentArea>
{
	static Quantity<ApartmentArea::Unit> Value() { return T::Instance().AreaQuantity(); }
};

template<typename T>
struct GetQuantity<T, Persons>
{
	static Quantity<Persons::Unit> Value() { return T::Instance().PersonsQuantity(); }
};

template<typename T>
struct GetQuantity<T, IndividualUnit>
{
	static Quantity<IndividualUnit::Unit> Value() { return T::Instance().UnitsQuantity(); }
};

using Top = Stage<TopConfiguration>;
using Middle = Stage<MiddleConfiguration>;
using Bottom = Stage<BottomConfiguration>;

#endif
