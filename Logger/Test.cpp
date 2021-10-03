#include <iostream>
#include "Logger.hpp"

int main()
{       
    std::cout<<"START"<<std::endl;
	
	Logger::Log()<<"Test"<<std::endl;
	Logger::Log()<<"Test2"<<std::endl;
	
	std::cout<<"END"<<std::endl;

    return 0;
}
