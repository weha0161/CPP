#include <string>
#include <iostream>
#include "../Unit/Unit.h"
#include "../Unit/SIPrefix.hpp"

#ifndef QUANTITY_H
#define QUANTITY_H

template<typename U, typename SiPrefix = SIPrefix<0>,typename T = double>
struct Quantity
{
	explicit Quantity(const T& v): value(v * SiPrefix::Factor) {}
	
	T Value() const { return value / SiPrefix::Factor;}
	T PureValue() const { return value;}
	using ValueType = T;
	using UnitType = U;
	using UnitPrefix = SiPrefix;
	using Type = Quantity<U,SiPrefix,T>;	

	template<typename U2 = U>
	Quantity(Quantity<U2,T> q ):value(q.Value()){ 	}
	
	Quantity<U,SiPrefix,T> operator+(const Quantity<U,SiPrefix,T>& y) { return Quantity<U,SiPrefix,T>(this->Value() + y.Value()); }
	
	template<typename SiPrefix2 = SiPrefix>
	Quantity<U,SiPrefix,T> operator+(const Quantity<U,SiPrefix2,T>& y) { return Quantity<U,SiPrefix,T>((this->PureValue() + y.Value())/ SiPrefix::Factor); }
	
	Quantity<U,SiPrefix,T> operator-(const Quantity<U,SiPrefix,T>& y) { return Quantity<U,SiPrefix,T>(this->Value() - y.Value()); }
	
	template<typename SiPrefix2 = SiPrefix>
	Quantity<U,SiPrefix,T> operator-(const Quantity<U,SiPrefix2,T>& y) { return Quantity<U,SiPrefix,T>((this->PureValue() - y.Value())/ SiPrefix::Factor); }
	
	Quantity<typename Transform<U, U,MultiplyPolicy>::Type, SiPrefix,T> operator*(const Quantity<U,SiPrefix,T>& q ){ 
		return Quantity<typename Transform<U, U, MultiplyPolicy>::Type, SiPrefix,T>(this->Value() * q.Value());
	}
	
	Quantity<typename Transform<U, U,DividePolicy>::Type, SiPrefix,T> operator/(const Quantity<U,SiPrefix,T>& q ){ 
		return Quantity<typename Transform<U, U, DividePolicy>::Type, SiPrefix,T>(this->Value() / q.Value());
	}
	
	template<typename U2 = U, typename SiPrefix2 = SiPrefix>
	Quantity<typename Transform<U, U2,MultiplyPolicy>::Type, SiPrefix,T> operator*(const Quantity<U2, SiPrefix2,T>& q ){ 
		return Quantity<typename Transform< U, U2, MultiplyPolicy>::Type, SiPrefix,T>(this->Value() * q.Value());
	}
	
	template<typename U2 = U, typename SiPrefix2 = SiPrefix>
	Quantity<typename Transform<U, U2,DividePolicy>::Type, SiPrefix,	T> operator/(const Quantity<U2,SiPrefix2,T>& q ){ 
		return Quantity<typename Transform< U, U2, DividePolicy>::Type, SiPrefix,T>(this->Value() / q.Value());
	}
private:
	T value;
	
};

template<typename U, typename T = double>
std::ostream& operator<<(std::ostream& out, const Quantity<U,T>& q)
{
	return out<<q.Value()<<" "<<U::Sum::Sign;
}


#endif
