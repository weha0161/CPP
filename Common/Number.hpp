
#ifndef NUMBER_HPP
#define NUMBER_HPP

namespace Number
{
	template <unsigned long long N, size_t base=10>
	struct Digits
	{
	    enum { Value = 1 + Digits<N/base, base>::Value };
	};
	
	template <size_t base>
	struct Digits<0, base>
	{
	    enum { Value = 0 };
	};
}


#endif
