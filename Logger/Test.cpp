#include <iostream>
#include "Logger.hpp"
#include "Logger.hpp"
#include <typeinfo>
#include <typeindex>
#include "../Wrapper/Wrapper.hpp"


int main()
{       
    std::cout<<"START"<<std::endl;
	
	Logger::Log()<<"Test"<<std::endl;
	Logger::Log<Error>()<<"Test2"<<std::endl;
	Logger::Log<Info>()<<"Test2"<<std::endl;
	
	LogExpression le1{1};
	LogExpression le2{2};
	LogExpression le3{3};
	std::cout<<le1.Value()<<std::endl;
	std::cout<<le2.Value()<<std::endl;
	std::cout<<le3.Value()<<std::endl;
	
	auto le = le1<<le2;
	auto le4 = le<<le3;
	auto le5 = le4<<le1<<le4<<le1<<le2;
	
	Logger::Log(1,2,3.9,"Test");
	Logger::Log<Error>(1,2,3.9,"Test");
	Logger::Log<';', Debug >(1,2,3.9,"Test");
	
	std::cout<<typeid(le5).name()<<std::endl;
	
	std::cout<<"END"<<std::endl;

    return 0;
}
