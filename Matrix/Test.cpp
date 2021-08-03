#include <iostream>
#include <boost/mpl/vector.hpp>
#include "Matrix.hpp"
#include "../Traits/Traits.h"

namespace mpl=boost::mpl;

int main()
{       
    std::cout<<"START"<<std::endl;
	
	using Gen = Generator<int>;
	
	Array<Gen> a = Array<Gen>(5,5);

    std::cout<<"START2"<<std::endl;
	a.InitElements("Test");
  
    std::cout<<"(3,3) :"<<a.Get(3,3)<<std::endl;
    std::cout<<"END"<<std::endl;

    return 0;
}
