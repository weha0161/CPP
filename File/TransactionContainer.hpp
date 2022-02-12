#include <memory>
#include <vector>
#include <algorithm>    // std::find
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
		
		Iterator operator[](int i) { return this->Begin(); }
		Iterator operator[](std::string s) { return this->Begin(); }
		Iterator operator[](Date s) { return this->Begin(); }
		
		const Iterator Begin() const { return this->transactions.cbegin(); }
		const Iterator End() const { return this->transactions.cend(); }
		
		void Add(T t){ this->transactions.push_back(t); }
	private:
		Iterator begin;
		Iterator end;
		ContainerType transactions;
	};
}


#endif
