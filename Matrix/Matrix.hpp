#include "../String/String_.h"
#include "../Logger/Logger.h"
#include "../Wrapper/Wrapper.h"
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/vector.hpp>
#include <map>

#ifndef CSV_H
#define CSV_H

//--------------------------------FORMAT_NON_ZERO------------------------------------------------

template<class Matrix>
struct FORMAT_NON_ZERO
{
	using Type = T::TrueType;
};

//--------------------------------BoundsChecker------------------------------------------------

template<class Array>
class ArrayFormat 
{
private:
	Array elemnts_;
public:
	using Config = Array::Config;
	using IndexType = Config::IndexType;
	using ElementType = Config::ElementType;
	using MatrixType = Config::MatrixType;
	
	ArrayFormat(const IndexType& rows, const IndexType& cols): elemnts_(rows, cols) { }
	
	const IndexType& rows() const { return elemnts_.rows(); }
	const IndexType& cols() const { return elemnts_.cols(); }
	
	void Set(const IndexType& i, const IndexType& j, const ElementType& v)
	{
		checkBounds(i, j);
		if(nonZero(i,j)) elemnts_.Set(i,j,v);
		else assert(v == ElementType(0));
	}
	
	ElementType Get(const IndexType& i, const IndexType& j) const 
	{
		checkBounds(i, j);
		return nonZero(i,j) ? elemnts_.Get(i,j) : ElementType(0);
	}
	
	void InitElements(const ElementType& v){elemnts_.InitElements(v);}
	
protected:
	void checkBounds(const IndexType& i, const IndexType& j) const
	{
		assert(i < rows());
		assert(j < cols());
	}
	
	bool nonZero(const IndexType& i, const IndexType& j) const
	{
		return FORMAT_NON_ZERO<MatrixType>::Type::Value;
	}
};

//--------------------------------BoundsChecker------------------------------------------------

template<class OptMatrix>
class BoundsChecker: public OptMatrix
{
public:
	using Config = OptMatrix::Config;
	using IndexType = Config::IndexType;
	using ElementType = Config::ElementType;
	
	BoundsChecker(const IndexType& rows, const IndexType& cols): OptMatrix(rows, cols) { }
	void Set(const IndexType& i, const IndexType& j, const ElementType& v)
	{
		checkBounds(i, j);
		OptMatrix::Set(i,j,v);
	}
	ElementType Get(const IndexType& i, const IndexType& j) const 
	{
		checkBounds(i, j);
		return OptMatrix::Get(i,j);
	}
	
protected:
	void checkBounds(const IndexType& i, const IndexType& j) const
	{
		if(i < 0 || i >= OptMatrix::rows() || j < 0 || j >= OptMatrix::cols())
			throw "Indices out of bounds";
	}
};

//--------------------------------Matrix------------------------------------------------

template<class CheckedMatrix>
class Matrix: public CheckedMatrix
{
public:
	using Config = CheckedMatrix::Config;
	using IndexType = Config::IndexType;
	using ElementType = Config::ElementType;
	using CommaInitializer = Config::CommaInitializer;
	
	Matrix(IndexType rows = 0, IndexType cols = 0, ElementType InitElement = ElementType(0)): CheckedMatrix(rows, cols) { InitElements(InitElement); }
	CommaInitializer operator=(const ElementType& v){ return CommaInitializer(*this,v); }
};


#endif
