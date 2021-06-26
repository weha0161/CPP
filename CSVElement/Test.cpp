#include <iostream>
#include "CSVElement.hpp"
#include "../Traits/Traits.h"

int main()
{       
    std::cout<<"START"<<std::endl;
	using Types = Typelist<Key,Value>;
	AbstractFactory<Types> a;
	Key* k = new Key();
	Value* v = new Value();
	
 	k->Test();
	v->Test();
	
    std::cout<<"AbstractFactory"<<std::endl;
	FactoryUnit<Key>* k2 = a.Create<Key>();
	k2->Test();
// 	k2::Type k3;
	
   
	
    std::cout<<"END"<<std::endl;

    return 0;
}
