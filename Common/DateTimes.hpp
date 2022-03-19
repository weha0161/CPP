#include <memory>
#include <ratio>
#include <chrono>

#ifndef DATETIMES_HPP
#define DATETIMES_HPP

namespace DateTimes
{
	template<typename T>
	struct DateTimeBase
	{
		using Derived = std::shared_ptr<T>;
		Derived Next() const { return std::make_shared<T>(this->value + 1); };
		Derived Prev() const { return std::make_shared<T>(this->value - 1); };
		std::string ToString() const { return ""; };
		uint Value() const { return this->value; }
		constexpr DateTimeBase(uint v):value{v} {}
		bool operator==(uint i){ return this->value == i;};
		bool operator==(int i){ return this->value == i;};
	protected:
		const uint value;
	};

	struct Month: DateTimeBase<Month>
	{
		using Base = DateTimeBase<Month>;
		static Base::Derived Get(uint i) { return std::make_shared<Month>(i);}
		constexpr Month(uint v): DateTimeBase<Month>(c(v)){};
		constexpr Month(int v): DateTimeBase<Month>(c((uint)v)){};
	private:
		constexpr uint c(uint i)
		{
			if( i > 12) return 1;
			if( i < 1) return 12;
			return i;
		}
	};
	
	struct Year: DateTimeBase<Year>
	{
		using Base = DateTimeBase<Year>;
		static Base::Derived Get(uint i) { return std::make_shared<Year>(i);}
		static Base::Derived Get(int i) { return std::make_shared<Year>((uint)i);}
		Year(uint v): DateTimeBase<Year>(v){};
		
	};
	
	struct Day: DateTimeBase<Day>
	{
		using Base = DateTimeBase<Day>;
		static Base::Derived Get(uint i) { return std::make_shared<Day>(i);}
		static Base::Derived Get(int i) { return std::make_shared<Day>((uint)i);}
		Day(uint v): DateTimeBase<Day>(v){};
	};
	
	inline static std::shared_ptr<Month> January = std::make_shared<Month>(1);
	inline static std::shared_ptr<Month> February= std::make_shared<Month>(2);
	inline static std::shared_ptr<Month> March= std::make_shared<Month>(3);
	inline static std::shared_ptr<Month> April= std::make_shared<Month>(4);
	inline static std::shared_ptr<Month> May= std::make_shared<Month>(5);
	inline static std::shared_ptr<Month> June= std::make_shared<Month>(6);
	inline static std::shared_ptr<Month> July= std::make_shared<Month>(7);
	inline static std::shared_ptr<Month> August= std::make_shared<Month>(8);
	inline static std::shared_ptr<Month> September= std::make_shared<Month>(9);
	inline static std::shared_ptr<Month> October= std::make_shared<Month>(10);
	inline static std::shared_ptr<Month> November= std::make_shared<Month>(11);
	inline static std::shared_ptr<Month> December= std::make_shared<Month>(12);

}


#endif
