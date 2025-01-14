#include <iostream>
#include <vector>
#include "Calculator.hpp"
#include "../Home/Counter.hpp"
#include "../Home/CounterContainer.hpp"
#include "../Home/StageContainer.hpp"
#include "../Home/Stage.hpp"

int main()
{       
    std::cout<<"START"<<std::endl;
    
	std::cout<<"---------------------READ------------------------"<<std::endl;
	CounterConatinerType::Instance().Read();
	StageContainerType::Instance();
	
	std::cout<<"---------------------CE1------------------------"<<std::endl;
	Logger::Log(CE1::Begin(),CE1::End());
	
	auto rd = Calculator::Counter<CE1,Calculator::Difference>::Calculate();
	Logger::Log(rd.begin(),rd.end());
	
	auto rs =Calculator::Counter<CE1,Calculator::Sum>::Calculate();
	Logger::Log(rs.begin(),rs.end());
	
	Logger::Log()<<Middle::Instance().UnitsQuantity()<<std::endl;
	auto rat =Calculator::Ratio::Calculate(Middle::Instance().UnitsQuantity(),Top::Instance().UnitsQuantity(), Bottom::Instance().MonthlyRentQuantity());
	Logger::Log()<<rat.Value<<std::endl;
	
	std::cout<<"END"<<std::endl;
    return 0;
}
