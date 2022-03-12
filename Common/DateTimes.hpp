#include <memory>
#include <ratio>
#include <chrono>

#ifndef DATETIMES_HPP
#define DATETIMES_HPP

namespace Common
{
	template<typename T>
	struct DateTimeBase
	{
		using Derived = std::shared_ptr<T>;
		Derived Next() const { return std::make_shared<T>(this->value + 1); };
		Derived Prev() const { return std::make_shared<T>(this->value - 1); };
		std::string ToString() const { return ""; };
		uint Value() const { return this->value; }
		constexpr DateTimeBase(uint v):value(v) {}
	protected:
// 		TP tp;
		const uint value;
	};

	struct Month: DateTimeBase<Month>
	{
		using Base = DateTimeBase<Month>;
		static Base::Derived Get(uint i) { return std::make_shared<Month>(i);}
		constexpr Month(uint v): DateTimeBase<Month>(v){};
	};
	
	struct Year: DateTimeBase<Year>
	{
		using Base = DateTimeBase<Year>;
		static Base::Derived Get(uint i) { return std::make_shared<Year>(i);}
		Year(uint v): DateTimeBase<Year>(v){};
	};
	
	inline constexpr Month January{1};
	inline constexpr Month February{2};
	inline constexpr Month March{3};
	inline constexpr Month April{4};
	inline constexpr Month May{5};
	inline constexpr Month June{6};
	inline constexpr Month July{7};
	inline constexpr Month August{8};
	inline constexpr Month September{9};
	inline constexpr Month October{10};
	inline constexpr Month November{11};
	inline constexpr Month December{12};

}


#endif
