#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "MatrixExpressionTemplates.hpp"
#include "Generator.hpp"
#include <cassert>

#ifndef MATRIX_H
#define MATRIX_H

//--------------------------------FORMAT_NON_ZERO------------------------------------------------

template<class Matrix>
struct FORMAT_NON_ZERO
{
	using Type = T::TrueType;
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
	Container elements;
	Row rows;

public:
	Array(const IndexType& row, const IndexType& col): row(row), col( col) 
	{ 
		assert(row > 0);
		assert(col > 0);
		
		this->elements = Container(row);
		for(IndexType i = 0;i < Rows() ; ++i)
		{
			this->elements.at(i) = Row(Cols());
		}
	}
	
	template<class A>
	Array(const Array<A>& a){ 	}
	
	template<class A>
	Array& operator=(const Array<A>& a){ 		return *this; 	}
	
	~Array(){	}
	
	const IndexType& Rows() const { return row; }
	const IndexType& Cols() const { return col; }
	
	void Set(const IndexType& i, const IndexType& j, const ElementType& v)
	{
		checkBounds(i, j);
		elements.at(i).at(j) = v; 
	}
	
	ElementType Get(const IndexType& i, const IndexType& j) const 
	{
		checkBounds(i, j);
		return elements.at(i).at(j);
	}
	
	
	void InitElements(const ElementType& v)
	{
		for(IndexType i = 0;i < Rows() ; ++i)
			for(IndexType j = 0; j < Cols() ; ++j)
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
		if(i < 0 || i >= OptMatrix::Rows() || j < 0 || j >= OptMatrix::Cols())
			throw "Indices out of bounds";
	}
};

template<class Generator>
class DenseCCommaInitializer
{
public:
	using Config = Generator::Config;
	using MatrixType = Config::MatrixType;
	using IndexType = Config::IndexType;
	using ElementType = Config::ElementType;
	
private:
	MatrixType& matrix_;
	IndexType i_, j_;
	
public:
	DenseCCommaInitializer(MatrixType& m, const ElementType& first): matrix_(m), i_(0), j_(0) { insert(first);}
	DenseCCommaInitializer& operator,(const ElementType& val)
	{ 
		insert(val);
		return *this;
	}
	
protected:
	void insert(const ElementType& v)
	{
		assert(j_ < matrix_.Rows());
		assert(i_ < matrix_.Cols());
		matrix_.Set(i_, j_, v);
		++j_;
		if(j_==matrix_.Cols())
		{
			j_=0;
			++i_;
		}
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
	
	Matrix(IndexType rows = 0, IndexType cols = 0, ElementType InitElement = ElementType()): CheckedMatrix(rows, cols) { this->InitElements(InitElement); }
	
	template<class A>
	Matrix(const Matrix<A>& m){ 	}
	
	~Matrix()	{ 	}
	bool constexpr ScalarMultiplication =  false;
	template<class Expr>
	Matrix(const BinaryExpression<Expr>& expr): CheckedMatrix(expr.Rows(), expr.Cols())	{ 		expr.Assign(this);}
	
	CommaInitializer operator=(const ElementType& v){	return CommaInitializer(*this,v); }	
	
	template<class A>
	Matrix& operator=(const Matrix<A>& m)
	{ 
		Logger::Log<Debug>()<<"Matrix& operator=(const Matrix<A>& m)"<<std::endl;
		MATRIX_ASSIGMENT<A>::RET::assign(this, &m);
		return *this; 
	}
	
	std::ostream& Display(std::ostream& out) const
	{
		for(IndexType i = 0;i < this->Rows() ; ++i)
		{
			for(IndexType j = 0; j < this->Cols() ; ++j)
				out<<this->Get(i,j)<<" ";
			out<<std::endl;
		}

		return out;
	}
private:	
	template<class Expr>
	Matrix& operator=(const BinaryExpression<Expr>& expr)
	{ 
		expr.Assign(this);
		return *this; 
	}
	
	template<class C, class D>
	Matrix& operator=(const AdditionExpression<C,D> expr) {	return *this; }
};

#endif
