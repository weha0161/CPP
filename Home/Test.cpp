#include <iostream>
#include <boost/mpl/vector.hpp>
#include "../Unit/SIPrefix.hpp"
#include "Counter.hpp"
#include "../Tuple/Tuple.hpp"
#include "CounterContainer.hpp"
#include "../Traits/Traits.h"
#include "../Unit/Unit.h"
#include "../Logger/Logger.hpp"

int main()
{       
    std::cout<<"START"<<std::endl;
	
    std::cout<<"Ostream "<<std::endl;
    std::cout<<CounterConatinerType::Instance()<<std::endl;
	
    std::cout<<"---------------------WRITE---------------------"<<std::endl;
	CounterConatinerType::Instance().Write();

    std::cout<<"---------------------READ------------------------"<<std::endl;
	CounterConatinerType::Instance().Read();
	
    std::cout<<"UNIT"<<std::endl;
	using s = Sum;
	using work = Work;
// 	auto cw = CWA(); 
		
    std::cout<<"Sum "<<Sum::SiUnit()<<std::endl;
    std::cout<<"Work "<<Work::SiUnit()<<std::endl;
	
    std::cout<<"Type "<<CWA::Type::Name<<std::endl;
    std::cout<<"Unit "<<CWA::Unit::SiUnit()<<std::endl;

	std::cout<<"Kilo "<<Kilo::Name<<std::endl;
    std::cout<<"Kilo "<<Kilo::Sign<<std::endl;
	
	std::cout<<"Hekto "<<Hekto::Name<<std::endl;
    std::cout<<"Hekto "<<Hekto::Sign<<std::endl;
	
	std::cout<<"Deka "<<Deka::Name<<std::endl;
    std::cout<<"Deka "<<Deka::Sign<<std::endl;
	auto ctw = CounterConatinerType::Instance().Get<CTHW>();
	
	Logger::Log(ctw.Begin(), ctw.End());
	
    std::cout<<"END"<<std::endl;

    return 0;
}
