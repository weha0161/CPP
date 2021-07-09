#include "../String/String_.h"
#include "../Logger/Logger.h"
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/vector.hpp>
#include <map>

#ifndef CSV_H
#define CSV_H

//--------------------------------Element------------------------------------------------
class Element
{
public:
	inline static const std::string Identifier = "";
	virtual Element* DoCreate() = 0;
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
	inline static const std::string Identifier = "Key";
	Key(std::string s = ""): Element(s) {};
	Key* DoCreate(){return this;};
};

class Value: public Element
{
public:
	inline static const std::string Identifier = "Value";
	Value(std::string s = ""): Element(s){};
	Value* DoCreate(){return this;};
};

class Entry: public Element
{
public:
	inline static const std::string Identifier = "";
	Entry(std::string s): Element(s){};
	Entry* DoCreate(){return this;};
};

class Date: public Element
{
public:
	Date(std::string s): Element(s){};
	Date* DoCreate(){return this;};
};


class Item: public Element
{
public:
	inline static const std::string Identifier = "";
	Key key;
	Item(std::string s): Element(s), key(s){};
	Item* DoCreate(){return this;};
};



//--------------------------------Factory------------------------------------------------

using Elements = boost::mpl::vector<Key, Value, Entry, Date>;

template<class TList = Elements, class Unit = Element,typename T = std::string,class IdentifierType = std::string, template<class> class CreatePolicy = CreateElementNewPolicy>
class ElementFactory
{
	using ProductList = TList;
	using Creator = Unit* (*)(T);
	
private:
	using AssocMap = std::map<IdentifierType,Creator>;
	inline static AssocMap associations_ = std::map<IdentifierType,Creator>();
	
	struct Register
	{
		template<class Type>
		void operator()(Type) const
		{
			RegisterImpl(Type::Identifier, CreatePolicy<Type>::DoCreate);
			Logger::Instance().Log<Debug>(std::string("Register: " + Type::Identifier));
		};
	};
	
	static bool RegisterImpl(const IdentifierType& id, const Creator& creator)
	{
		return associations_.insert(std::make_pair(id,creator)).second;
	}
	
public:
	ElementFactory()
	{		
		boost::mpl::for_each<ProductList>(Register());
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
};

#endif
