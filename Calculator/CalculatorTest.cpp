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
	
	std::vector<int> v = std::vector<int>{1,2,3};
	
// 	auto c = Calculator<int, CE1>(v.cbegin(), v.cend());
	
// 	c.Calculate();
	
	std::cout<<"END"<<std::endl;
    return 0;
}
