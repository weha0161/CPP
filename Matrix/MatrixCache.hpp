#include "../String/String_.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "Generator.hpp"
#include <cassert>

#ifndef MATRIXCACHE_H
#define MATRIXCACHE_H

template<class ElementT>
class CacheElementType
{
public:
	using ElementType = ElementT;
	CacheElementType() : element(ElementT(0)), valid(false) {};
	
	CacheElementType(const ElementT& elem) : element(elem), valid(false) {};
	
	const bool& IsHit() const {return valid;}
	
	const ElementT& Get() const {return element;}
	
	void Set(const ElementT& elem)
	{
		valid = true;
		element = elem;
		Logger::Log<Debug>()<<"Set: "<<elem<<std::endl;
	};
	
	std::ostream& Display(std::ostream& out) const
	{
		out<<"("<<element<<"; "<<valid<<")";
		return out;
	};
	
private:
	bool valid;
	ElementT element;
};

template<class A>
std::ostream& operator<<(std::ostream& out, const CacheElementType<A> elem) { return elem.Display(out); }

template<class MatrixType>
class CACHE_MATRIX_TYPE
{
	using Config = typename MatrixType::Config;
	using DSLFeatures = typename Config::DSLFeatures;
	
	struct CachedMatrixDSL//: public DSLFeatures
	{
		using ElementType = CacheElementType<typename DSLFeatures::ElementType>;
	};
	
public:
// 	using RET = Generator<CachedMatrixDSL>::RET;
	using RET = CachedMatrixDSL::ElementType;
	using ElementType = CachedMatrixDSL::ElementType;
};


#endif
