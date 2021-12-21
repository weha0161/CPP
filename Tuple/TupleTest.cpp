#include <iostream>
#include "Tuple.hpp"

int main()
{       
    std::cout<<"START"<<std::endl;
	
	using TypeContainer = Tuple<long,double,int,char,bool>;
		
	auto tc = TypeContainer();
	
	auto cw4 = Get<bool>(tc);
	auto cw3 = Get<char>(tc);
	auto cw2 = Get<int>(tc);
	auto cw1 = Get<double>(tc);
	auto cw0 = Get<long>(tc);
	
	auto w4 = Get<4>(tc);
	auto w3 = Get<3>(tc);
	auto w2 = Get<2>(tc);
	auto w1 = Get<1>(tc);
	auto w0 = Get<0>(tc);
	
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
