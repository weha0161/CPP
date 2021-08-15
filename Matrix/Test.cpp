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
	using M = GenInt::Config::MatrixType;
	
	auto m1 = Matrix<Array<GenInt>>(5,5);
	m1.InitElements(5);
	
	std::cout<<m1<<std::endl;
	
	auto m2 = Matrix<Array<GenInt>>(5,5);
	auto m5 = M(2,2);
	m5 = 1,2,3,4;
	auto m6 = M(2,2);
	m6 = 1,2,3,4;
	m2.InitElements(3);
	
	std::cout<<m2<<std::endl;

	auto m3 = m1 + m2;	
	auto m4 = m1 + m2 + m2;	
	
// 	std::cout<<m3.Get(1,2)<<std::endl;
	std::cout<<m3<<std::endl;
	std::cout<<m4<<std::endl;
	std::cout<<m5<<std::endl;
	std::cout<<m6<<std::endl;
	
	auto m7 = m5 + m6;
	std::cout<<m7<<std::endl;
	
	std::cout<<m4.Get(0,1)<<std::endl;
	
	std::cout<<"END"<<std::endl;

    return 0;
}
