#include <iostream>
#include <vector>
#include "Calculator.hpp"
#include "../Home/CounterContainer.hpp"

int main()
{       
    std::cout<<"START"<<std::endl;
    
	std::cout<<"---------------------READ------------------------"<<std::endl;
	CtrContainer.Read();
	
	std::cout<<"---------------------CE1------------------------"<<std::endl;
	auto ce1 = CtrContainer.Get<CE1>();
	std::cout<<ce1<<std::endl;
	ce1.Calculate<Difference>();
	
	Logger::Log(ce1.Begin(),ce1.End());
	
	std::cout<<"END"<<std::endl;
    return 0;
}
