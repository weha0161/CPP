#include <iostream>
#include <boost/mpl/vector.hpp>
#include "Matrix.hpp"
#include "MatrixOperators.hpp"
#include <typeinfo>
#include <typeindex>
#include "../Traits/Traits.h"

namespace mpl=boost::mpl;

int main()
{       
    std::cout<<"START"<<std::endl;
	
	using Gen = Generator<int>;
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
	
	M m1 = M(2,2);
	M m3 = M(2,2);
	M m4 = M(2,2);
	M m2 = M(2,2);
	m1.InitElements(1);
	m2.InitElements(2);
	m3.InitElements(3);
	m4.InitElements(4);
	
 	std::cout<<"M1: \n"<<m1<<std::endl;
 	std::cout<<"M2: \n"<<m2<<std::endl;
 	std::cout<<"M3: \n"<<m3<<std::endl;
 	std::cout<<"M4: \n"<<m4<<std::endl;
	
 	std::cout<<"Additiom: \n"<<std::endl;
	auto m5 = (m1 + m1)  + (m1 + m1);
	std::cout<<"Get m3 (1,1): "<<m5->Get(0,0)<<std::endl;
	auto m6 = m1  + (m1 + m1);
	std::cout<<"Get m3 (1,1): "<<m6->Get(0,1)<<std::endl;
	auto m7 = m1 + m1 + m1 + m1;
	std::cout<<"Get m3 (1,1): "<<m7->Get(0,1)<<std::endl;
	
 	std::cout<<"Multiplication: \n"<<std::endl;
	
	auto m8 = (m1 * m1) *(m1 * m1);
	std::cout<<"Get m3 (1,1): "<<m8->Get(0,0)<<std::endl;
	auto m9 = m1 *(m1 * m1);
	std::cout<<"Get m3 (1,1): "<<m9->Get(0,1)<<std::endl;
	auto m10 = m1 * m1 * m1 * m1;
	std::cout<<"Get m3 (1,1): "<<m10->Get(0,1)<<std::endl;
	
 	std::cout<<"Multiplication Scalar: \n"<<std::endl;
	
	auto s = T::int_<-1>();
	std::cout<<"Get m3 (1,1): "<<s.Value<<std::endl;
	auto m11 = s  * m1;
	std::cout<<"Get m3 (1,1): "<<m10->Get(0,1)<<std::endl;
	std::cout<<"END"<<std::endl;

    return 0;
}
