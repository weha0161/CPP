#include <iostream>
#include <boost/mpl/vector.hpp>
#include "Counter.hpp"
#include "../Traits/Traits.h"
#include "../Unit/Unit.h"

namespace mpl=boost::mpl;

int main()
{       
    std::cout<<"START"<<std::endl;
	
	using W = CounterConfiguration<Water>;
	using CW = Counter<W>;
	using s = Sum;
	using work = Work;
	auto cw = CW("W1"); 
		
    std::cout<<"Sum "<<Sum::UnitSign()<<std::endl;
    std::cout<<"Work "<<Work::UnitSign()<<std::endl;
	
    std::cout<<"Type "<<CW::Type::Name<<std::endl;
    std::cout<<"Unit "<<CW::Unit::UnitSign()<<std::endl;
    std::cout<<"Name "<<cw.Name()<<std::endl;
    std::cout<<"END"<<std::endl;

    return 0;
}
