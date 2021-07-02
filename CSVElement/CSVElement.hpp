#include "../String/String_.h"
#include <map>

#ifndef CSVELEMENT_H
#define CSVELEMENT_H


class CSVElement
{
public:
	virtual CSVElement* DoCreate() = 0;
	virtual void Do() = 0;
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
	static CSVElement* DoCreate(T param)
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
	void Do(){ std::cout<<"Key"<<std::endl; };
};

class Value: public CSVElement
{
public:
	Value(std::string s): CSVElement(s){};
	Value* DoCreate(){return this;};
	void Do(){ std::cout<<"Value"<<std::endl; };
};

//--------------------------------Factory------------------------------------------------


template<class TList, class Unit = CSVElement,typename T = std::string,class IdentifierType = int, template<class> class CreatePolicy = CreateCSVElementNewPolicy>
class CSVElementFactory
{
public:
	using ProductList = TList;
	typedef Unit* (*Creator)(T);
// 	using Creator = Unit* (*)(T);
	
	CSVElementFactory()
	{
// 		Register(1, &Create<Key, std::string>);
		IdentifierType id = 0;
		associations_.insert(std::make_pair(id, &CreatePolicy<Key>::DoCreate));
		
// 		associations_[id] = &CreatePolicy<Key>::DoCreate;
	}
	
	bool Register(const IdentifierType& id,Creator creator)
	{
		return associations_.insert(id,creator).second;
	}
	
	bool Unregister(const IdentifierType& id)
	{
		return associations_.erase(id) == 1;
	}
	
	template<class Type> 
	Unit* Create(T param)
	{
		Unit* unit = CreatePolicy<Type>::DoCreate(param);
		return unit->DoCreate();
	}
	
	Unit* Create(const IdentifierType& id,T param)
	{
// 		typename AssocMap::const_iterator i = associations_.find(id);
// 		
// 		if(i != associations_.end)
// 		{
// 			return (id->second)(param);
// 		}
		
			return (associations_.find(id)->second)(param);
	}
	
	
private:
	using AssocMap = std::map<IdentifierType,Creator>;
	AssocMap associations_;
};

#endif
