#include "../String/String_.h"
#include <map>

#ifndef CSV_H
#define CSV_H


//--------------------------------Element------------------------------------------------
class Element
{
public:
	inline static const std::string Identifier = "";
	virtual Element* DoCreate() = 0;
	virtual void Do() = 0;
	virtual ~Element(){}
	
	Element(std::string s):stringValue(s), Value(s) {};
	const std::string Value;
private:
	const String_ stringValue;
};

//--------------------------------CreateElementNewPolicy------------------------------------------------
template<class ConcreteProduct, typename T = std::string>
class CreateElementNewPolicy
{
public:
	static Element* DoCreate(T param)
	{
		return new ConcreteProduct(param);
	}
};


//--------------------------------TYPES------------------------------------------------


class Key: public Element
{
public:
	inline static const std::string Identifier = "";
	Key(std::string s): Element(s) {};
	Key* DoCreate(){return this;};
	void Do(){ std::cout<<"Key"<<std::endl; };
};

class Value: public Element
{
public:
	inline static const std::string Identifier = "";
	Value(std::string s): Element(s){};
	Value* DoCreate(){return this;};
	void Do(){ std::cout<<"Value"<<std::endl; };
};

class Entry: public Element
{
public:
	inline static const std::string Identifier = "";
	Entry(std::string s): Element(s){};
	Entry* DoCreate(){return this;};
	void Do(){ std::cout<<"Value"<<std::endl; };
};

class Date: public Element
{
public:
	Date(std::string s): Element(s){};
	Date* DoCreate(){return this;};
	void Do(){ std::cout<<"Value"<<std::endl; };
};

class Item: public Element
{
public:
	inline static const std::string Identifier = "";
	Key key;
	Item(std::string s): Element(s), key(s){};
	Item* DoCreate(){return this;};
	void Do(){ std::cout<<"Value"<<std::endl; };
};



//--------------------------------Factory------------------------------------------------


template<class TList, class Unit = Element,typename T = std::string,class IdentifierType = int, template<class> class CreatePolicy = CreateElementNewPolicy>
class ElementFactory
{
public:
	using ProductList = TList;
	using Creator = Unit* (*)(T);
	
	ElementFactory()
	{
		IdentifierType id = 0;
		Register(id, CreatePolicy<Key>::DoCreate);
		id = 1;
		Register(id, CreatePolicy<Value>::DoCreate);		
	}
	
	bool Register(const IdentifierType& id, const Creator& creator)
	{
		return associations_.insert(std::make_pair(id,creator)).second;
	}
	
	bool Unregister(const IdentifierType& id)
	{
		return associations_.erase(id) == 1;
	}
		
	Unit* Create(const IdentifierType& id,T param)
	{
		typename AssocMap::const_iterator i = associations_.find(id);
		
		if(i != associations_.end())
		{
			return (i->second)(param);
		}
		
		return (associations_.find(id)->second)(param);
	}
	
	
private:
	using AssocMap = std::map<IdentifierType,Creator>;
	AssocMap associations_;
	
	
// 	template<typename List>
// 	Element* Get(const IdentifierType& id)
// 	{
// 		using Type = Front<List>;
// 		
// 		if(Type::ID == id)
// 			return id;
// 		
// 		return Get<PopFront<List>>(id);
// 	};
// 	
// 	template<typename List>
// 	Element* Get<>(const IdentifierType& id)
// 	{
// 		if(Type::ID == id)
// 			return id;
// 		
// 	}
};

#endif
