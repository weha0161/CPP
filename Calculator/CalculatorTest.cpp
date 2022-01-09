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
	Calculator<CE1,Difference<CE1>>::Calculate();
	
	Logger::Log(CE1::Begin(),CE1::End());
	
	std::cout<<"END"<<std::endl;
    return 0;
}
