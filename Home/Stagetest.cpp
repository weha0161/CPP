#include <iostream>
#include <boost/mpl/vector.hpp>
#include "Counter.hpp"
#include "CounterContainer.hpp"
#include "Stage.hpp"
#include "StageContainer.hpp"
#include "../Unit/SIPrefix.hpp"
#include "../Calculator/Calculator.hpp"
#include "../Tuple/Tuple.hpp"
#include "../Traits/Traits.h"
#include "../Unit/Unit.h"
#include "../Logger/Logger.hpp"

int main()
{       
    std::cout<<"START"<<std::endl;
	
// 	Top::Instance();
    std::cout<<"t: "<<Top::Name<<std::endl;
    std::cout<<"t: "<<Top::Number<<std::endl;
    
    std::cout<<"t: "<<ApartmentArea::Key<<std::endl;
	
	ApartmentArea aa(7);
	Rooms r(6);
	
	StageContainerType::Instance().Display(std::cout);
// 	auto m = StageContainerType::Instance().Get<Middle>();
    std::cout<<"m: "<<Middle::Instance().Name<<std::endl;
    
    std::cout<<"AREA"<<std::endl;
	std::cout<<"t: "<<Get<Top,ApartmentArea>::Value()<<std::endl;
    std::cout<<"m: "<<Get<Middle,ApartmentArea>::Value()<<std::endl;
    std::cout<<"b: "<<Get<Bottom,ApartmentArea>::Value()<<std::endl;
    std::cout<<"TOTAL: "<<StageContainerType::Instance().GetTotal<ApartmentArea>()<<std::endl;
	
    std::cout<<"\nPersons"<<std::endl;
	std::cout<<"t: "<<Get<Top,Persons>::Value()<<std::endl;
    std::cout<<"m: "<<Get<Middle,Persons>::Value()<<std::endl;
    std::cout<<"b: "<<Get<Bottom,Persons>::Value()<<std::endl;
    std::cout<<"TOTAL: "<<StageContainerType::Instance().GetTotal<Persons>()<<std::endl;
	
    std::cout<<"END"<<std::endl;

    return 0;
}
