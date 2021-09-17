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
	CacheElementType() : element(ElementType(0)), valid(false) {};
	
	CacheElementType(const ElementType& elem) : element(elem), valid(false) {};
	
	const bool& IsHit() const {return valid;}
	
	const ElementType& Get() const {return element;}
	
	void Set(const ElementType& elem)
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
	ElementType element;
};

template<class A>
std::ostream& operator<<(std::ostream& out, const CacheElementType<A> elem) { return elem.Display(out); }

template<class MatrixType>
class CACHE_MATRIX_TYPE
{
	using Config = typename MatrixType::Config;
	using DSLFeatures = typename Config::DSLFeatures;
	
	struct CachedMatrixDSL: public DSLFeatures
	{
// 		using ElementType = CacheElementType<typename Config::MatrixType>;
		using ElementType = CacheElementType<typename Config::ElementType>;
	};
	
public:
	using RET = Generator<CachedMatrixDSL>::RET;
// 	using RET = CachedMatrixDSL::ElementType;
// 	using ElementType = CachedMatrixDSL::ElementType;
};


#endif
