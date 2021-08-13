#include "../String/String_.h"
#include "../Logger/Logger.h"
#include "../Wrapper/Wrapper.h"
#include "MatrixExpressionTemplates.hpp"
#include "Generator.hpp"
#include <cassert>

#ifndef MATRIXCACHE_H
#define MATRIXCACHE_H

template<class ElementType>
class CacheElementType
{
public:
	CacheElementType() : element(ElementType(0)), valid(false) {};
	
	CacheElementType(const ElementType& elem) : element(elem), valid(false) {};
	
	const bool& IsHit() const {return valid;}
	
	const ElementType& Get() const {return element;}
	
	void Set(const ElementType& elem)
	{
		valid = true;
		element = elem;
	}
	
	ostream& Display(ostream& out) const
	{
		out<<"("<<element<<"; "<<valid<<")";
		return out;
	}
	
private:
	bool valid;
	ElementType element;
}

template<class A>
ostream& operator<<(ostream& ou, const CacheElementType<A> elem) { return elem.Display(out;) }


#endif
