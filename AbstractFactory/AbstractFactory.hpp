#ifndef ABSTRACTFACTORY_H
#define ABSTRACTFACTORY_H

template<class T>
class FactoryUnit
{
public:
	using Type = T;
	virtual T* DoCreate() = 0;
	virtual void Test() = 0;
	virtual ~FactoryUnit(){} 
};

template<class ConcreteProduct>
class CreateFactoryUnitNewPolicy
{
public:
	static ConcreteProduct* DoCreate()
	{
		return new ConcreteProduct;
	}
};

template<class TList, template<class> class Unit = FactoryUnit, template<class> class CreatePolicy = CreateFactoryUnitNewPolicy>
class AbstractFactory
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
template<class TList, class Factory = AbstractFactory<TList>, template<class,class> class Creator = CreateFactoryUnitNewPolicy>
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
