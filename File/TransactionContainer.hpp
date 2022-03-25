#include <memory>
#include <vector>
#include <algorithm>    // std::find
#include <chrono>    // std::find
#include "../Logger/Logger.hpp"
#include "../Common/DateTimes.hpp"
#include "../Common/Algorithms.hpp"
#include "../String/String_.hpp"
#include "../CSV/CSV.hpp"

#ifndef ACCOUNTTRANSFERSCONTAINER_HPP
#define ACCOUNTTRANSFERSCONTAINER_HPP

namespace Bank
{
// 	using MonthType = std::chrono::month;
// 	using YearType = std::chrono::year;
	
	using MonthType = std::shared_ptr<DateTimes::Month>;
	using YearType = std::shared_ptr<DateTimes::Year>;
	
	template<typename T>
	class TransactionContainer
	{
	public:
		using Type = TransactionContainer<T> ;
		using ContainerType = std::vector<T>;
		using ContainerPtr = std::shared_ptr<ContainerType>;
		using TypePtr = std::shared_ptr<Type>;
		using Iterator = ContainerType::const_iterator;

		TransactionContainer(): transactions{std::make_shared<ContainerType>()}{}
		
// 		Type operator[](std::string s) { return Type(ContainerType(this->Begin()+1, this->End()-1)); }
		Type operator[](DateTimes::Date s) 
		{ 
			return Type(); 
		}
		
		Type operator[](int i) 
		{ 
			return Type(); 
			
		}
		
		template<typename KeyT>
		TypePtr operator[](KeyT t) 
		{ 
			auto result = std::make_shared<Type>();
			std::for_each(this->Begin(), this->End(),[&](auto i){ if(*i == t) result->Add(i);});
			
			return result;  
		}
		
		const Iterator Begin() const { return this->transactions->cbegin(); }
		const Iterator End() const { return this->transactions->cend(); }
		const size_t Size() const { return this->transactions->size(); }
		void Add(T t){ this->transactions->push_back(t); }
		void Display()
		{ 
			for(auto i : *(this->transactions))
			Logger::Log()<<*i<<std::endl;
		}
	private:
		ContainerPtr transactions;
		TransactionContainer(ContainerPtr c): transactions(c){ Logger::Log()<<"TransactionContainer: "<<*this->transactions.at(0)<<"\t"<<*this->transactions.at(0)<<std::endl;	}
	};
}


#endif
