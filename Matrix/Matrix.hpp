#include "../String/String_.h"
#include "../Logger/Logger.h"
#include "../Wrapper/Wrapper.h"
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/vector.hpp>
#include <map>
#include <cassert>

#ifndef CSV_H
#define CSV_H

//--------------------------------FORMAT_NON_ZERO------------------------------------------------

template<class Matrix>
struct FORMAT_NON_ZERO
{
	using Type = T::TrueType;
};

template<typename IndexT = int,
			typename ElementT = std::string,
			template<typename> class ContainerType = std::vector,
			typename RowT = ContainerType<ElementT>,
			typename ContainerT = ContainerType<RowT>>
struct Generator
{
private:
	struct Configuration
	{
		using IndexType = IndexT;
		using ElementType = ElementT;
		using Container = ContainerT;
		using Row = RowT;
	};
public:
	using Config = Configuration;
};

//--------------------------------Array------------------------------------------------

template<class Generator>
class Array 
{
public:
	using Config = Generator::Config;
	using IndexType = Config::IndexType;
	using ElementType = Config::ElementType;
	using Container = Config::Container;
	using Row = Config::Row;

protected:
	IndexType row, col;
	Container* elements;
	Row* rows;

public:
	Array(const IndexType& row, const IndexType& col): row(row), col( col) 
	{ 
		assert(row > 0);
		assert(col > 0);
		elements = new Container();
	}
	
	~Array()
	{
		delete elements;
		delete rows;
	}
	
	const IndexType& Rows() const { return row; }
	const IndexType& Cols() const { return col; }
	
	void Set(const IndexType& i, const IndexType& j, const ElementType& v)
	{
		checkBounds(i, j);
	}
	
	ElementType Get(const IndexType& i, const IndexType& j) const 
	{
		checkBounds(i, j);
		return (elements->at(i)).at(j);
	}
	
	
	void InitElements(const ElementType& v)
	{
		for(IndexType i = Rows(); --i;)
			for(IndexType j = Cols(); --j;)
				Set(i,j,v);
	}
	
private:
	void checkBounds(const IndexType& i, const IndexType& j) const
	{
		assert(i < Rows());
		assert(j < Cols());
	}
};

//--------------------------------Format------------------------------------------------

template<class Array>
class ArrayFormat 
{
	
public:
	using Config = Array::Config;
	using IndexType = Config::IndexType;
	using ElementType = Config::ElementType;
	using MatrixType = Config::MatrixType;
private:
	Array elemnts_;
	
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
