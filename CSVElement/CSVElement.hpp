#include "../String/String_.h"

#ifndef CSVELEMENT_H
#define CSVELEMENT_H


template<class T>
class CSVElement
{
public:
	using Type = T;
	virtual T* DoCreate() = 0;
	virtual void Test() = 0;
	virtual ~CSVElement(){} 
private:
	String_ Value;
};

template<class ConcreteProduct>
class CreateCSVElementNewPolicy
{
public:
	static ConcreteProduct* DoCreate()
	{
		return new ConcreteProduct;
	}
};


//--------------------------------TYPES------------------------------------------------


class Key: public virtual FactoryUnit<Key>
{
public:
// 	FactoryUnit<Key>* DoCreate(){return this;};
	Key* DoCreate(){return this;};
	void Test(){ std::cout<<"KEY"<<std::endl; };
};

class Value: public virtual FactoryUnit<Value>
{
public:
	Value* DoCreate(){return this;};
	void Test(){ std::cout<<"VALUE"<<std::endl;};
};

//--------------------------------Factory------------------------------------------------


template<class TList, template<class> class Unit = CSVElement, template<class> class CreatePolicy = CreateCSVElementNewPolicy>
class CSVElementFactory
{
public:
	using ProductList = TList;
	
	template<class T> 
	T* Create()
	{
		Unit<T>* unit = CreatePolicy<T>::DoCreate();
		return unit->DoCreate();
	}
};

/*
template<class TList, class Factory = CSVElementFactory<TList>, template<class,class> class Creator = CreateCSVElementNewPolicy>
class ConcreteFactory
{
public:
	using ProductList = TList;
	
	template<class T> 
	T* Create()
	{
		Unit<T>* unit = new T();
		return unit->DoCreate();
	}	
};*/


#endif
