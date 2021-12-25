#include <iostream>
#include "Wrapper.hpp"

int main()
{       
    std::cout<<"START"<<std::endl;
	
	using i57 = T::int_<57>::Type;
	using i58 = T::int_<58>;
	using i70 = T::int_<70>;
//     std::cout<<int_<1>::Value<<" "<<(int)i1<<" "<<std::endl;
//     std::cout<<int_<1>::Value<<" "<<(int)i2<<" "<<std::endl;
	
	using c1 = T::char_<55>;
	using c2 = T::char_<87>;
	using c3 = T::char_<-1>;
    std::cout<<c1::Value<<" "<<c2::Value<<" "<<std::endl;
//     std::cout<<c1::ValueType().Type::Value<<" "<<c2::ValueType().Type::Value<<" "<<std::endl;
//     std::cout<<c3::ValueType().Type::Value<<" "<<c3::ValueType().Type::Value<<" "<<std::endl;
	
// 	using n57 = Num<i1>;
// 	using n58 = Num<i2>;

    std::cout<<i57::Value<<std::endl;
	T::ValueType<i57> v57;
	T::ValueType<i70> v70;
	
	using e = Traits::EnableIf<T::IsLetter<i70>::Value, T::Letter<i70>>::Type;
    std::cout<<e::Value<<std::endl;
    std::cout<<e::Next::Value<<std::endl;
	using e2 = Traits::EnableIf<T::IsNum<i57>::Value, T::Letter<i57>>::Type;
    std::cout<<e2::Value<<std::endl;
    std::cout<<e2::Previous::Value<<std::endl;
// 	EnableIf<false, int> i3 = 57;
	
	using wl = VL::Valuelist<char, 'a', 'b', 'c'>;
	std::cout<<VL::Front<wl>::Value<<std::endl;
	using pf =VL::PopFront<wl>::Type;
	std::cout<<VL::PopFront<wl>::Value<<std::endl;
	std::cout<<VL::Front<pf>::Value<<std::endl;
	
	using puf =VL::PushFront<wl, 'x'>::Type;
	std::cout<<VL::Front<puf>::Value<<std::endl;
	
	using pub =VL::PushBack<wl, 'y'>::Type;
	std::cout<<VL::Front<puf>::Value<<std::endl;
	
	using pf1 =VL::PopFront<wl>::Type;
	std::cout<<pf1::Size<<std::endl;
	using pf2 =VL::PopFront<pf1>::Type;
	std::cout<<pf2::Size<<std::endl;
	
	using t = VL::Transform<char, T::char_, 'a','b'>::Type;
	using t2 = VL::Transform<int, T::int_, 1,2,3>::Type;
	using tb = VL::Transform<bool, T::bool_, false,true,false,false,true,false>::Type;
	
	std::cout<<"TL char size: "<<t::Size<<std::endl;
	std::cout<<"TL int size: "<<t2::Size<<std::endl;
	std::cout<<"TL bool size: "<<tb::Size<<std::endl;
	const char c = 'v';
	
// 	VL::PushBack<char, c , x> vl;
	/*
	for(int i = 0; i < 10; ++i)
	{
		VL::PushBack<char, c , 'x'> vl;
	}*/
	
    std::cout<<"END"<<std::endl;

    return 0;
}
