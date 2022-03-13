#include <memory>
#include <vector>
#include <algorithm>    // std::find
#include <chrono>    // std::find
#include "../Logger/Logger.hpp"
#include "../Common/DateTimes.hpp"
#include "../String/String_.hpp"
#include "../CSV/CSV.hpp"

#ifndef ACCOUNTTRANSFERSCONTAINER_HPP
#define ACCOUNTTRANSFERSCONTAINER_HPP

namespace Bank
{
// 	using MonthType = std::chrono::month;
// 	using YearType = std::chrono::year;
	
	using MonthType = std::shared_ptr<Common::Month>;
	using YearType = std::shared_ptr<Common::Year>;
	
	template<typename T>
	class TransactionContainer
	{
	public:
		using Type = TransactionContainer<T> ;
		using ContainerType = std::vector<T>;
		using Iterator = ContainerType::const_iterator;

		TransactionContainer(){
			this->transactions = ContainerType();
		}
		
		Type operator[](std::string s) { return Type(ContainerType(this->Begin()+1, this->End()-1)); }
		Type operator[](Date s) { return Type(); }
		Type operator[](int i) 
		{ 
			ContainerType result;
			for(auto t : this->transactions)
				if((*(t->GetDate().Month())) == (uint)i)
					result.push_back(t);
				
			Logger::Log()<<result.at(0);
			return Type(this->transactions); 
			
		}
		Type operator[](MonthType m) { return Type(this->transactions); }
		Type operator[](YearType y) { return Type(this->transactions); }
		
		const Iterator Begin() const { return this->transactions.cbegin(); }
		const Iterator End() const { return this->transactions.cend(); }
		const size_t Size() const { return this->transactions.size(); }
		void Add(T t){ this->transactions.push_back(t); }
	private:
		ContainerType transactions;
		TransactionContainer(ContainerType c): transactions(ContainerType(c)){ Logger::Log()<<"TransactionContainer: "<<*this->transactions.at(0)<<"\t"<<*this->transactions.at(0)<<std::endl;	}
	};
}


#endif
