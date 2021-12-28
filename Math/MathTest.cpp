#include <iostream>
#include "Math.hpp"

int main()
{       
    std::cout<<"START"<<std::endl;
	
    std::cout<<"\n>0"<<std::endl;
	std::cout<<Pow<10,1>::Result<< std::endl;
	std::cout<<Pow<10,2>::Result<< std::endl;
	std::cout<<Pow<10,3>::Result<< std::endl;
    
    std::cout<<"\n=0"<<std::endl;
	std::cout<<Pow<10,0>::Result<< std::endl;
	
    std::cout<<"\n<0"<<std::endl;
	std::cout<<Pow<10,-1>::Result<< std::endl;
	std::cout<<Pow<10,-2>::Result<< std::endl;
	std::cout<<Pow<10,-3>::Result<< std::endl;
    
	std::cout<<"END"<< std::endl;

    return 0;
}
