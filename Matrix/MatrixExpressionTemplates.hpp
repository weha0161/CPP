#include "Generator.hpp"

#ifndef MATRIXEXPRESSIONTEMPLATES_H
#define MATRIXEXPRESSIONTEMPLATES_H

template<class CheckedMatrix>
class Matrix;

template<class ExpressionType>
class BinaryExpression; 

//----------------------------------------------------------------------------------------------------AdditionExpression--------------------------------------

template<class A, class B>
struct ADD_RESULT_TYPE
{
	using RET = Generator<int>;
};

struct RectAssignment
{
	template<class Res, class M>
	static void assign(Res* res, M* m)
	{
		std::cout<<"ASSIGN";
		using IndexType = Res::Config::IndexType;
		for(IndexType i = 0;i < m->Rows() ; ++i)
			for(IndexType j = 0; j < m->Cols() ; ++j)
				res->Set(i,j,m->Get(i,j));
	}
};


template<class A>
struct MATRIX_ASSIGMENT
{
	using RET = RectAssignment;
};

struct RectAddGetElement
{
	template<class IndexType, class ResultType, class LeftType, class RightType>
	
	static typename ResultType::ElementType Get(const IndexType& i, const IndexType& j, const ResultType* res, const LeftType& leftType, const RightType& rightType)
	{
		return leftType.Get(i,j) + rightType.Get(i,j);
	}
};

template<class A, class B>
struct MATRIX_ADD_GET_ELEMENT
{
	using RET = RectAddGetElement;
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
		if(m1.Cols() != m2.Cols() || m1.Rows() != m2.Rows()) throw "argument matrices are incompatible";
	}
	
	ElementType Get(const IndexType& i, const IndexType& j) const
	{
		return MATRIX_ADD_GET_ELEMENT<LeftType, RightType>::RET::Get(i, j, this, left_, right_);
	}
	
	IndexType Rows() const { return rows_ ;}
	IndexType Cols() const { return cols_ ;}
};

//----------------------------------------------------------------------------------------------------MultiplicationExpression--------------------------------------

// template<class A, class B>
// struct ADD_RESULT_TYPE
// {
// 	using RET = Generator<int>;
// };
// 
// struct RectAssignment
// {
// 	template<class Res, class M>
// 	static void assign(Res* res, M* m)
// 	{
// 		std::cout<<"ASSIGN";
// 		using IndexType = Res::Config::IndexType;
// 		for(IndexType i = 0;i < m->Rows() ; ++i)
// 			for(IndexType j = 0; j < m->Cols() ; ++j)
// 				res->Set(i,j,m->Get(i,j));
// 	}
// };
/*

template<class A>
struct MATRIX_ASSIGMENT
{
	using RET = RectAssignment;
};*/

template<class MatrixType>
class CACHE_MATRIX_TYPE
{
	using Config = MatrixType::Config;
	using RET = Generator<int>;
};

struct RectMultiplyGetElement
{
	template<class IndexType_, class ResultType, class LeftType, class RightType, class LeftCacheMatrixType, class RightCacheMatrixType>
	static typename ResultType::ElementType Get(const IndexType_& i, const IndexType_& j, const ResultType* res, const LeftType& leftType, const RightType& rightType, LeftCacheMatrixType leftCache = 0, RightCacheMatrixType rightCache = 0)
	{
		using Config = ResultType::Config;
		using ElementType = Config::ElementType;
		using IndexType = Config::IndexType;
		
		ElementType result = ElementType(0);
		
		for(IndexType k = leftType.Cols(); --k;)
			result += getCachedElement(i,k,leftType,leftCache) * getCachedElement(k,j,rightType,rightCache);
		
		return leftType.Get(i,j) + rightType.Get(i,j);
	}
};

template<class A, class B>
struct MATRIX_MULTIPLY_GET_ELEMENT
{
	using RET = RectMultiplyGetElement;
};

template<class A, class B>
class MultiplicationExpression
{
public: 
	using LeftType = A;
	using RightType = B;
	
	using LeftMatrixType = LeftType::Config::MatrixType;
	using RightMatrixType = RightType::Config::MatrixType;
	
	using Config = ADD_RESULT_TYPE<LeftType,RightType>::RET::Config;
	using ElementType = Config::ElementType;
	using IndexType = Config::IndexType;
	
private:
	using LeftCacheMatrixType = CACHE_MATRIX_TYPE<LeftMatrixType>::RET;
	using RightCacheMatrixType = CACHE_MATRIX_TYPE<RightMatrixType>::RET;
	
	const LeftType& left_;
	const RightType& right_;
	
	LeftCacheMatrixType* leftCacheMatrix_;
	RightCacheMatrixType* rightCacheMatrix_;
	
protected:
	const IndexType rows_, cols_;
	
public:
	template<class M1, class M2>
	MultiplicationExpression(const Matrix<M1>& m1, const Matrix<M2>& m2): left_(m1), leftCacheMatrix_(0), rightCacheMatrix_(0),right_(m2), rows_(m1.Rows()), cols_(m2.Cols())
	{
		ParameterCheck(m1, m2);
	}
	
	template<class Expr, class M2>
	MultiplicationExpression(const BinaryExpression<Expr>& expr, const Matrix<M2>& m2): left_(expr), right_(m2), rightCacheMatrix_(0) , rows_(expr.Rows()), cols_(m2.Cols())
	{
		ParameterCheck(expr, m2);
		leftCacheMatrix_=new LeftMatrixType(expr.Rows(), expr.Cols());
	}
	
	template<class M1, class Expr>
	MultiplicationExpression(const Matrix<M1>& m1, const BinaryExpression<Expr> expr): left_(m1), leftCacheMatrix_(0), right_(expr), rows_(m1.Rows()), cols_(expr.Cols())
	{
		ParameterCheck(m1, expr);
		leftCacheMatrix_=new LeftMatrixType(expr.Rows(), expr.Cols());
	}
	
	template<class Expr1, class Expr2>
	MultiplicationExpression(const BinaryExpression<Expr1>& expr1, const BinaryExpression<Expr2> expr2): left_(expr1), right_(expr2), rows_(expr1.Rows()), cols_(expr2.Cols())
	{
		ParameterCheck(expr1, expr2);
		leftCacheMatrix_=new LeftMatrixType(expr1.Rows(), expr1.Cols());
		rightCacheMatrix_=new RightCacheMatrixType(expr2.Rows(), expr2.Cols());
	}	
	
	ElementType Get(const IndexType& i, const IndexType& j) const
	{
		return MATRIX_ADD_GET_ELEMENT<LeftType, RightType>::RET::Get(i, j, this, left_, right_);
	}
	
	IndexType Rows() const { return rows_ ;}
	IndexType Cols() const { return cols_ ;}
	
private:
	void ParameterCheck(const A& m1, const B& m2)
	{
		if(m1.Cols() != m2.Rows())
			throw "argument matrices are incompatible!";
	}
};

//----------------------------------------------------------------------------------------------------BinaryExpression--------------------------------------

template<class ExpressionType>
class BinaryExpression : public ExpressionType
{
public:
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

#endif
