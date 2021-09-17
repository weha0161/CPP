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
	M m10 = M(2,2);
	auto m2 = m10;
	m1.InitElements(1);
	m3.InitElements(2);
	m4.InitElements(4);
	m10.InitElements(2);
	
// 	std::cout<<m1<<std::endl;
// 	std::cout<<m10<<std::endl;
	
// 	auto m2 = M(5,5);
	M m5 = M(2,2);
// 	m5 = 1,2,3,4;
// 	auto m6 = M(2,2);
// 	m6 = 1,2,3,4;
// 	m2.InitElements(4);
	
// 	std::cout<<m1<<std::endl;

// 	auto m6 = (m1 + m3) + m4;	
	M m6 = (m1 + m3) + m4 + m4;	
// 	BinaryExpression<AdditionExpression<Matrix<M>, Matrix<M>>> m6 = (m1 + m3);	
// 	M m6 = (m1 + m3);	
	
// 	M m = M(m3);
// 	M mA = M(2,2);	
// 	M mB = M(2,2);	
	
// 	mA = m3;
	std::cout<<"ASSIGNMENT"<<std::endl;
	std::cout<<"Get m3 (1,1): "<<m6.Get(1,1)<<std::endl;
// 	std::cout<<"Get m=m3 (1,1): "<<m.Get(1,1)<<std::endl;
// 	mB = mA;
	
// 	auto m9 = m10 * m10;
// 	std::cout<<"Get m3 (1,1): "<<m9.Get(1,1)<<std::endl;
	
// 	std::cout<<m3<<std::endl;
// 	std::cout<<"M4: \n"<<m4<<std::endl;
// 	std::cout<<m9<<std::endl;
// 	std::cout<<"M4 Get: \n"<<m4.Get(1,1)<<std::endl;
// 	std::cout<<"M9 Get: \n"<<m9.Get(1,1)<<std::endl;
	
// 	std::cout<<m5<<std::endl;
// 	std::cout<<m6<<std::endl;
	
// 	auto m7 = m5 + m6;
// 	auto m8 = m5 + m6 + m7;
// 	std::cout<<m7<<std::endl;
// 	std::cout<<m8<<std::endl;
	
// 	std::cout<<"Multiplication: \n"<<m9<<std::endl;
	
	
	std::cout<<"END"<<std::endl;

    return 0;
}
