#include <iostream>
#include <vector>
#include "../Logger/Logger.hpp"
#include "DateTimes.hpp"

int main()
{       
    std::cout<<"START"<<std::endl;
    auto j = Common::Month::Get(1);
	Logger::Log()<<j->Value()<<std::endl;
	Logger::Log()<<j->Next()->Value()<<std::endl;
	std::cout<<"END"<<std::endl;
    
	auto y = Common::Month::Get(2021);
	Logger::Log()<<y->Value()<<std::endl;
	Logger::Log()<<y->Prev()->Value()<<std::endl;
	
	Logger::Log()<<Common::July.Value()<<std::endl;
	Logger::Log()<<Common::July.Prev()->Value()<<std::endl;
	std::cout<<"END"<<std::endl;
    
	return 0;
}
