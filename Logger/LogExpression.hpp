#ifndef LOGEXPR_HPP
#define LOGEXPR_HPP

template<typename T = std::string>
class LogExpression
{
	public:
		using ValueType = T;
		LogExpression(ValueType v): value{v} {} 
	
		const ValueType Value() const { return this->value; }
	private:
		const ValueType value;
};

template<typename E1, typename E2>
class LogSumExpression
{
	public:
		LogSumExpression(const E1& e1, const E2& e2): e1{e1}, e2{e2} {} 
	
		decltype(auto) Value() const { return e1.Value() + e2.Value(); }
	private:
		const E1& e1;
		const E2& e2;
};

//~ template<typename T>
//~ inline LogSumExpression<LogExpression<T>,LogExpression<T>> operator<<( const LogExpression<T>& e1, const LogExpression<T>& e2)	{ std::cout<<"LogExp e1 "<<e1.Value()<<" e2 "<<e2.Value()<<std::endl;	return {e1,e2}; }

//~ template<typename E1, typename E2>
//~ inline LogSumExpression<E1,E2> operator<<( const E1& e1, const E2& e2)	{ std::cout<<"LogSum e1 "<<e1.Value()<<" e2 "<<e2.Value()<<std::endl; return {e1,e2}; }

//~ template<typename E>
//~ inline std::ostream& operator<<( std::ostream& os, const E& e)	{ return os<<e.Value(); }
#endif
