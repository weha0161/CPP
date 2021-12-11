#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/vector.hpp>
#include <map>
#include <chrono>
#include <ctime>

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
	const std::string stringValue;
};

std::ostream& operator<<(std::ostream& out, const Element& e)
{
	return out<<e.Value;
}

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

inline bool operator< (const Key& lhs, const Key& rhs){ return lhs.Value < rhs.Value; }

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
	inline static const std::string Identifier = "Entry";
	Entry(std::string s): Element(s){};
	Entry* DoCreate(){return this;};
};

class Date: public Element
{
	std::chrono::system_clock::time_point CreateTimePoint(int y, int m, int d)
	{
		struct std::tm t;
		t.tm_sec = 0;
		t.tm_min = 0;
		t.tm_hour = 0;
		t.tm_mday = d;
		t.tm_mon = m-1;
		t.tm_year = y - 1900;
		
		std::time_t tt = std::mktime(&t);
		
		return std::chrono::system_clock::from_time_t(tt);
	}
	
	void Parse(std::string s)
	{
		std::string res;
		for(auto c : s)
			if(isdigit(c))
				res += c;
		
		Logger::Log<Debug>()<<res<<std::endl;
		
		int d = std::stoi(std::string(res.begin(),res.begin()+2));
		int m = std::stoi(std::string(res.begin()+3,res.begin()+4));
		int y = std::stoi(std::string(res.begin()+4,res.begin()+8));
		
		Logger::Log<Debug>()<<d<<"-"<<m<<"-"<<y<<std::endl;
		this->tp = this->CreateTimePoint(y,m,d);
	}
	
	std::chrono::system_clock::time_point tp;
	
// 	std::string TimeString(const std::chrono::system_clock::time_point& t)
public:
	inline static const std::string Identifier = "Date";
	Date(std::string s): Element(s){ this->Parse(s); };
	Date* DoCreate(){return this;};

	std::string TimeString()
	{
		std::time_t t = std::chrono::system_clock::to_time_t(this->tp);
		std::string ts = ctime(&t);
		ts.resize(ts.size()-1);
		return ts;
	}
};

class IBAN: public Element
{
public:
	inline static const std::string Identifier = "IBAN";
	IBAN(std::string s): Element(s){ };
	IBAN* DoCreate(){return this;};
};

class BIC: public Element
{
public:
	inline static const std::string Identifier = "BIC";
	BIC(std::string s): Element(s){};
	BIC* DoCreate(){return this;};
};


class Item: public Element
{
public:
	inline static const std::string Identifier = "Item";
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
			Logger::Log<Debug>()<<"Register: "<< Type::Identifier<<std::endl;
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
