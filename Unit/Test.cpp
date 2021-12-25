#include <iostream>
#include "Unit.h"

int main()
{       
    std::cout<<"START"<<std::endl;
    std::cout<<Mass::Mass::Name<<" "<<Mass::Mass::Sign<<" "<<Mass::Mass::N<<" "<<std::endl;
    std::cout<<Sum::Sum::Name<<" "<<Sum::Sum::Sign<<" "<<std::endl;
    std::cout<<Length::Length::Name<<" "<<Length::Length::Sign<<" "<<std::endl;
	
	auto u1 = Mass();
	auto u2 = Unit<0,0,2>();
// 	auto u3 = Transform<u1::Type,u2::Type, MultiplyPolicy>();
	using u3 = Transform<Sum,Unit<2>, MultiplyPolicy>::Type;

	std::cout<<u3::Sum::N<<std::endl;
	std::cout<<UnitSign<int, 0, 0,0, 0,0, 0,0, 0>::Get()<<std::endl;
	std::cout<<UnitSign<Mass>::Get()<<std::endl;
	std::cout<<UnitSign<decltype(u2)>::Get()<<std::endl;
    std::cout<<"END"<<std::endl;
    return 0;
}
