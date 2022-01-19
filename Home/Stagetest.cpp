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
//     std::cout<<"t: "<<Top::Instance().Name<<std::endl;
    std::cout<<"t: "<<Top::AreaValue<<std::endl;
    std::cout<<"t: "<<Top::RoomsValue<<std::endl;
    std::cout<<"t: "<<Top::Number<<std::endl;
    
    std::cout<<"t: "<<ApartmentArea::Key<<std::endl;
	
	ApartmentArea aa(7);
	Rooms r(6);
	
	StageContainerType::Instance().Display(std::cout);
// 	auto m = StageContainerType::Instance().Get<Middle>();
    std::cout<<"m: "<<Middle::Instance().Name<<std::endl;
	
    std::cout<<"END"<<std::endl;

    return 0;
}
