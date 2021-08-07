#include "../String/String_.h"
#include "../Logger/Logger.h"
#include "../Wrapper/Wrapper.h"
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/vector.hpp>
#include <map>
#include <cassert>

#ifndef CSV_H
#define CSV_H

struct delete_ptr { // Helper function to ease cleanup of container
    template <typename P>
    void operator () (P p) {
        delete p;
    }
};

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
			typename ContainerT = ContainerType<RowT*>>
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
		
		this->elements = new Container(row);
		for(IndexType i = 0;i < Rows() ; ++i)
		{
			this->elements->at(i) =new Row(Cols());
		}
	}
	
	~Array()
	{
		std::for_each(this->elements->begin(), this->elements->end(), delete_ptr());
		this->elements->clear();
	}
	
	const IndexType& Rows() const { return row; }
	const IndexType& Cols() const { return col; }
	
	void Set(const IndexType& i, const IndexType& j, const ElementType& v)
	{
		checkBounds(i, j);
		elements->at(i)->at(j) = v; 
	}
	
	ElementType Get(const IndexType& i, const IndexType& j) const 
	{
		checkBounds(i, j);
		return elements->at(i)->at(j);
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
	IndexType& i_, j_;
	
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
// 	using CommaInitializer = Config::CommaInitializer;
	
	Matrix(IndexType rows = 0, IndexType cols = 0, ElementType InitElement = ElementType()): CheckedMatrix(rows, cols) { this->InitElements(InitElement); }
// 	CommaInitializer operator=(const ElementType& v){ return CommaInitializer(*this,v); }

// 	template<class Expr>
// 	Matrix& operator=(const BinaryExpression& b)
// 	{ 
// 		expr.assign(this);
// 		return *this; 
// 	}
// 	
// 	template<class A>
// 	Matrix& operator=(const Matrix<A>& m)
// 	{ 
// 		MATRIX_ASSIGMENT<A>::RET::assign(this, &m)
// 		return *this; 
// 	}
	
	std::ostream& display(std::ostream& out) const
	{
		for(IndexType i = 0;i < this->Rows() ; ++i)
		{
			for(IndexType j = 0; j < this->Cols() ; ++j)
				out<<this->Get(i,j)<<" ";
			out<<std::endl;
		}

		return out;
	}
};

struct RectAddGetElement
{
	template<class IndexType, class ResultType, class LeftType, class RightType>
	
	static typename ResultType::ElementType Get(const IndexType& i, const IndexType& j, const ResultType* res, const LeftType& leftType, const RightType& rightType)
	{
		return leftType.Get(i,j) + rightType.GetType(i,j);
	}
};

template<class A, class B>
struct ADD_RESULT_TYPE
{
	using RET = A;
};

struct RectAssignment
{
	template<class Res, class M>
	static void assign(Res* res, M* m)
	{
		using IndexType = Res::Config::IndexType;
		for(IndexType i = 0;i < m->Rows() ; ++i)
			for(IndexType j = 0; j < m->Cols() ; ++j)
				res-Set(i,j,m->Get(i,j));
	}
};


template<class A>
struct MATRIX_ASSIGMENT
{
	using RET = RectAssignment;
};

template<class A, class B>
struct MATRIX_ADD_GET_ELEMENT
{
	using RET = A;
};

template<class A, class B>
class AdditionExpression
{
public: 
	using LeftType = A;
	using RightType = B;
	
	using Config = ADD_RESULT_TYPE<LeftType,RightType>::RET::Config;
	using ElementType = Config::ElementType;
	using IndexType = Config::IndexType;
	
private:
	const LeftType& left_;
	const RightType& right_;
	
protected:
	const IndexType rows_, cols_;
	
public:
	AdditionExpression(const LeftType& m1, const RightType& m2): left_(m1), right_(m2), rows_(m1.Rows()), cols_(m1.Cols())
	{
		if(m1.cols() != m2.cols() || m1.rows() != m2.rows()) throw "argument matrices are incompatible";
	}
	
	ElementType Get(const IndexType& i, const IndexType& j) const
	{
		return MATRIX_ADD_GET_ELEMENT<LeftType, RightType>::RET::getElement(i, j, this, left_, right_);
	}
	
	IndexType Rows() const { return rows_ ;}
	IndexType Cols() const { return cols_ ;}
};

template<class ExpressionType>
class BinaryExpression : public ExpressionType
{
	using LeftType = ExpressionType::LeftType;
	using RightType = ExpressionType::RightType;
	using MatrixType = ExpressionType::Config::MatrixType;
	using IndexType = ExpressionType::IndexType;
	
	BinaryExpression(const LeftType& op1, const RightType& op2): ExpressionType(op1, op2){};
	
	template<class Res>
	Matrix<Res> assign(Matrix<Res> const result) const
	{
		MATRIX_ASSIGMENT<MatrixType>::RET::assign(result, this);
		return result;
	}
	
	std::ostream& display(std::ostream& out) const
	{
		for(IndexType i = 0;i < this->Rows() ; ++i)
		{
			for(IndexType j = 0; j < this->Cols() ; ++j)
				out<<this->Get(i,j)<<" ";
			out<<std::endl;
		}

		return out;
	}
};

template<class A>
std::ostream& operator<<(std::ostream& out, const Matrix<A> m)
{
	return m.display(out);
}

template<class M1, class M2> 
inline BinaryExpression<AdditionExpression<Matrix<M1>, Matrix<M2>>> operator+(const Matrix<M1>& m1, const Matrix<M2>& m2)
{
	return BinaryExpression<AdditionExpression<Matrix<M1>, Matrix<M2>>>(m1, m2);
}

template<class Expr, class M> 
inline BinaryExpression<AdditionExpression<BinaryExpression<Expr>, Matrix<M>>> operator+(const BinaryExpression<Expr>& expr, const Matrix<M>& m)
{
	return BinaryExpression<AdditionExpression<BinaryExpression<Expr>, Matrix<M>>>(expr, m);
}

template<class Expr1, class Expr2> 
inline BinaryExpression<AdditionExpression<BinaryExpression<Expr1>, BinaryExpression<Expr2>>> operator+(const BinaryExpression<Expr1>& expr1, const BinaryExpression<Expr2>& expr2)
{
	return BinaryExpression<AdditionExpression<BinaryExpression<Expr1>, BinaryExpression<Expr2>>>(expr1, expr2);
}

#endif
