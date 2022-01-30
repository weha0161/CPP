#include <memory>
#include <ratio>

#ifndef CALCULATORRESULT_HPP
#define CALCULATORRESULT_HPP

namespace Calculator
{
	template<typename TReading, typename TQuantity>
	struct Result			
	{
		const TReading FirstReading;
		const TReading SecondReading;
		const TQuantity Value;
		
		template<typename Separator = T::char_<'\t'>>
		std::ostream& Display(std::ostream& out) const
		{
			return out<<FirstReading<<Separator::Value<<SecondReading<<Separator::Value<<Value<<std::endl;
		}
		
		Result(TReading r1, TReading r2, TQuantity q): FirstReading(r1), SecondReading(r2), Value(q) {};	
	};

	template<typename R, typename Q, typename S = T::char_<'\t'>>
	std::ostream& operator<<(std::ostream& strm, const Result<R,Q> cr)
	{
		return cr.Display(strm);
	}
}


#endif
