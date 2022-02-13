#include <memory>
#include <vector>
#include <algorithm>    // std::find
#include <chrono>    // std::find
#include "../Logger/Logger.hpp"
#include "../String/String_.hpp"
#include "../CSV/CSV.hpp"

#ifndef ACCOUNTTRANSFERSCONTAINER_HPP
#define ACCOUNTTRANSFERSCONTAINER_HPP

namespace Bank
{
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
		
		Type operator[](int i) { return Type(this->transactions); }
		Type operator[](std::string s) { return Type(ContainerType(this->Begin()+1, this->End()-1)); }
		Type operator[](Date s) { return Type(); }
// 		Type operator[](std::chrono::month m) { return Type(); }
// 		Type operator[](std::chrono::year y) { return Type(); }
		
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
