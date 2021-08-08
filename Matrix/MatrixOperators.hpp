#include "../String/String_.h"
#include "../Logger/Logger.h"
#include "../Wrapper/Wrapper.h"
#include "MatrixExpressionTemplates.hpp"
#include "Matrix.hpp"

#ifndef MATRIXOPERATORS_H
#define MATRIXOPERATORS_H

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
inline BinaryExpression<AdditionExpression<BinaryExpression<Expr>, Matrix<M>>> operator+(const Matrix<M>& m, const BinaryExpression<Expr>& expr)
{
	return BinaryExpression<AdditionExpression<Matrix<M>,BinaryExpression<Expr>>>(expr, m);
}

template<class M, class Expr> 
inline BinaryExpression<AdditionExpression<Matrix<M>, BinaryExpression<Expr>>> operator+(const BinaryExpression<Expr>& expr, const Matrix<M>& m)
{
	return BinaryExpression<AdditionExpression<BinaryExpression<Expr>, Matrix<M>>>(expr, m);
}

template<class Expr1, class Expr2> 
inline BinaryExpression<AdditionExpression<BinaryExpression<Expr1>, BinaryExpression<Expr2>>> operator+(const BinaryExpression<Expr1>& expr1, const BinaryExpression<Expr2>& expr2)
{
	return BinaryExpression<AdditionExpression<BinaryExpression<Expr1>, BinaryExpression<Expr2>>>(expr1, expr2);
}

#endif
