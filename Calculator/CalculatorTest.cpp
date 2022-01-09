#include <iostream>
#include <vector>
#include "Calculator.hpp"
#include "../Home/Counter.hpp"
#include "../Home/CounterContainer.hpp"

int main()
{       
    std::cout<<"START"<<std::endl;
    
	std::cout<<"---------------------READ------------------------"<<std::endl;
	CounterConatinerType::Instance().Read();
	
	std::cout<<"---------------------CE1------------------------"<<std::endl;
// 	auto ce1 = CounterConatinerType::Instance().Get<CE1>();
// 	std::cout<<CE1<<std::endl;
	CE1::Calculate<Difference<CE1>>();
	
	Logger::Log(CE1::Begin(),CE1::End());
	
	std::cout<<"END"<<std::endl;
    return 0;
}
