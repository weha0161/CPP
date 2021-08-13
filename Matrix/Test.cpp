#include <iostream>
#include <boost/mpl/vector.hpp>
#include "Matrix.hpp"
#include "MatrixOperators.hpp"
#include "../Traits/Traits.h"

namespace mpl=boost::mpl;

int main()
{       
    std::cout<<"START"<<std::endl;
	
	using Gen = Generator<int>;
	
// 	Array<Gen> a = Array<Gen>(5,5);
// 
// 	a.InitElements("Test");
//   
//     std::cout<<"(3,3) :"<<a.Get(3,3)<<std::endl;
    
	auto b = BoundsChecker<Array<Gen>>(3,3);
	
	try
	{
		std::cout<<"(4,4) :"<<b.Get(4,3)<<std::endl;
	}
	catch(const char* c)
	{
		std::cout<<*c<<std::endl;
	}
	
	using GenInt = Generator<int,int>;
	
	auto m1 = Matrix<Array<GenInt>>(5,5);
	m1.InitElements(5);
	
	std::cout<<m1<<std::endl;
	
	auto m2 = Matrix<Array<GenInt>>(5,5);
	m2.InitElements(3);
	
	std::cout<<m2<<std::endl;

	auto m3 = m1 + m2;	
	auto m4 = m1 + m3 + m3;	
	
	std::cout<<m3.Get(1,2)<<std::endl;
	std::cout<<m4.Get(1,2)<<std::endl;
	
	std::cout<<"END"<<std::endl;

    return 0;
}
