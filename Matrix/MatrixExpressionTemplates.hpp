#include "../Logger/Logger.h"
#include "Generator.hpp"

#ifndef MATRIXEXPRESSIONTEMPLATES_H
#define MATRIXEXPRESSIONTEMPLATES_H

template<class CheckedMatrix>
class Matrix;

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
