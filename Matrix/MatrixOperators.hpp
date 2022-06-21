#include <memory>
#include "MatrixExpressionTemplates.hpp"
#include "../Logger/Logger.hpp"
#include "Matrix.hpp"

#ifndef MATRIXOPERATORS_H
#define MATRIXOPERATORS_H

template<class A>
std::ostream& operator<<(std::ostream& out, const Matrix<A>& m){ return m.Display(out);}

template<class A>
std::ostream& operator<<(std::ostream& out, const BinaryExpression<A>& m){	return m.Display(out);}

//AdditionExpression

template<class M1, class M2> 
inline decltype(auto) operator+(const Matrix<M1>& m1, const Matrix<M2>& m2)
{
	return std::make_shared<BinaryExpression<AdditionExpression<Matrix<M1>, Matrix<M2>>>>(m1, m2);
}

template<class Expr, class M> 
inline decltype(auto) operator+(std::shared_ptr<BinaryExpression<Expr>> expr, const Matrix<M>& m)
{
	return std::make_shared<BinaryExpression<AdditionExpression<BinaryExpression<Expr>, Matrix<M>>>>(*expr, m);
}

template<class M, class Expr> 
inline decltype(auto) operator+(Matrix<M>& m, std::shared_ptr<BinaryExpression<Expr>> expr)
{
	return std::make_shared<BinaryExpression<AdditionExpression<Matrix<M>,BinaryExpression<Expr>>>>(m, *expr);
}

template<class Expr1, class Expr2> 
inline decltype(auto) operator+(std::shared_ptr<BinaryExpression<Expr1>> expr1, std::shared_ptr<BinaryExpression<Expr2>> expr2)
{
	return std::make_shared<BinaryExpression<AdditionExpression<BinaryExpression<Expr1>, BinaryExpression<Expr2>>>>(*expr1, *expr2);
}

//MultiplicationExpression

template<class M1, class M2> 
inline decltype(auto) operator*(const Matrix<M1>& m1, const Matrix<M2>& m2)
{
	return std::make_shared<BinaryExpression<MultiplicationExpression<Matrix<M1>, Matrix<M2>>>>(m1, m2);
}

template<int N, class M2> 
inline decltype(auto) operator*(const T::int_<N>& s, const Matrix<M2>& m2)
{
	auto m1 = Matrix<M2>(m2.Rows(), m2.Cols());
	m1.InitElements(s.Value);
        m1.ScalarMultiplication =  true;
	return std::make_shared<BinaryExpression<MultiplicationExpression<Matrix<M2>, Matrix<M2>>>>(m1, m2);
}

template<class Expr, class M> 
inline decltype(auto) operator*(std::shared_ptr<BinaryExpression<Expr>> expr, const Matrix<M>& m)
{
	return std::make_shared<BinaryExpression<MultiplicationExpression<BinaryExpression<Expr>, Matrix<M>>>>(*expr, m);
}

template<class M, class Expr> 
inline decltype(auto) operator*(const Matrix<M>& m, std::shared_ptr<BinaryExpression<Expr>> expr)
{
	return std::make_shared<BinaryExpression<MultiplicationExpression<Matrix<M>,BinaryExpression<Expr>>>>(m, *expr);
}

template<class Expr1, class Expr2> 
inline decltype(auto) operator*(std::shared_ptr<BinaryExpression<Expr1>> expr1, std::shared_ptr<BinaryExpression<Expr2>> expr2)
{
	return std::make_shared<BinaryExpression<MultiplicationExpression<BinaryExpression<Expr1>, BinaryExpression<Expr2>>>>(*expr1, *expr2);
}

#endif
