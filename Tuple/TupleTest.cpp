#include <iostream>
#include "Tuple.hpp"

int main()
{       
    std::cout<<"START"<<std::endl;
	
	using TypeContainer = Tuple<long,double,int,char,bool>;
		
	auto tc = TypeContainer();
	
	auto cw4 = GetType<bool>(tc);
	auto cw3 = GetType<char>(tc);
	auto cw2 = GetType<int>(tc);
	auto cw1 = GetType<double>(tc);
	auto cw0 = GetType<long>(tc);
	
	auto w4 = GetNum<4>(tc);
	auto w3 = GetNum<3>(tc);
	auto w2 = GetNum<2>(tc);
	auto w1 = GetNum<1>(tc);
	auto w0 = GetNum<0>(tc);
	
    std::cout<<sizeof(cw4)<< std::endl;
    std::cout<<sizeof(cw3)<< std::endl;
    std::cout<<sizeof(cw2)<< std::endl;
    std::cout<<sizeof(cw1)<< std::endl;
    std::cout<<sizeof(cw0)<< std::endl;
    std::cout<<"---------------------------------"<< std::endl;
    
	
	std::cout<<sizeof(w4)<< std::endl;
    std::cout<<sizeof(w3)<< std::endl;
    std::cout<<sizeof(w2)<< std::endl;
    std::cout<<sizeof(w1)<< std::endl;
    std::cout<<sizeof(w0)<< std::endl;
	
	auto t2 = T::int_<2>();
    std::cout<<sizeof(tc[t2])<< std::endl;
	
    std::cout<<"END"<< std::endl;

    return 0;
}
