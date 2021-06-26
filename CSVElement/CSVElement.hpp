#include "../String/String_.h"

#ifndef CSVELEMENT_H
#define CSVELEMENT_H


class CSVElement
{
public:
	virtual CSVElement* DoCreate() = 0;
	virtual ~CSVElement(){}
	
	CSVElement(std::string s):stringValue(s), Value(s){};
	const std::string Value;
private:
	const String_ stringValue;
};

template<class ConcreteProduct, typename T = std::string>
class CreateCSVElementNewPolicy
{
public:
	static ConcreteProduct* DoCreate(T param)
	{
		return new ConcreteProduct(param);
	}
};


//--------------------------------TYPES------------------------------------------------


class Key: public CSVElement
{
public:
	Key(std::string s): CSVElement(s){};
	Key* DoCreate(){return this;};
};

class Value: public CSVElement
{
public:
	Value(std::string s): CSVElement(s){};
	Value* DoCreate(){return this;};
};

//--------------------------------Factory------------------------------------------------


template<class TList, class Unit = CSVElement, template<class> class CreatePolicy = CreateCSVElementNewPolicy>
class CSVElementFactory
{
public:
	using ProductList = TList;
	
	template<class Type, typename T = std::string> 
	Unit* Create(T param)
	{
		Unit* unit = CreatePolicy<Type>::DoCreate(param);
		return unit->DoCreate();
	}
};

#endif
