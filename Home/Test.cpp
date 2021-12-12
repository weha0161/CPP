#include <iostream>
#include <boost/mpl/vector.hpp>
#include "Counter.hpp"
#include "../Traits/Traits.h"

namespace mpl=boost::mpl;

int main()
{       
    std::cout<<"START"<<std::endl;
	
	using W = CounterConfiguration<Water>;
	using CW = Counter<W>;
	auto cw = CW("W1"); 
		
    std::cout<<"Type "<<CW::Type::Name<<std::endl;
    std::cout<<"Unit "<<CW::Unit::Sign()<<std::endl;
    std::cout<<"Name "<<cw.Name()<<std::endl;
    std::cout<<"END"<<std::endl;

    return 0;
}
