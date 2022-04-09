#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <memory>
#include "ParsedValues.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../Traits/Traits.h"
#include "../Logger/Logger.hpp"

#ifndef PARSERSTATE_HPP
#define PARSERSTATE_HPP

namespace String_
{
	struct ParserState
	{
		using Iterator = std::string::const_iterator;
		using ValueType = std::shared_ptr<ParsedValue>;
		using ContainerType = std::vector<ValueType>;
		
		Iterator Current() { return (this->value->cbegin() + ctr);}
		Iterator End() { return this->value->cend();}
		Iterator Begin() { return this->value->cbegin();}
		
		char CurrentVal() { return *(this->value->cbegin() + ctr);}
		void Increment() { ++ctr; }
		uint Counter(){ return this->ctr; }
		
		void Set(const std::string& s){ this->value = std::make_shared<std::string>(s);}
		std::shared_ptr<std::string> Get() const {  return this->value;}
		
		void Add(ValueType v)
		{ 
			if(this->parsedValues->size() > 1)
				v->setNext(*(this->parsedValues->cbegin()));
				
			this->parsedValues->push_back(v); 
		}
	private:
		uint ctr = 0;
		std::unique_ptr<ContainerType> parsedValues = std::make_unique<ContainerType>();
		std::shared_ptr<std::string> value = std::make_shared<std::string>();
	};
}

#endif
