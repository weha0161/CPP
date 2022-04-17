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
		using ContainerParaType = std::shared_ptr<ContainerType>;
		
		Iterator Current() { return (this->value->cbegin() + ctr);}
		Iterator End() { return this->value->cend();}
		Iterator Begin() { return this->value->cbegin();}
		
		char CurrentVal() { return *(this->value->cbegin() + ctr);}
		void Increment() { ++ctr; }
		uint Counter(){ return this->ctr; }
		
		void Set(std::shared_ptr<std::string> s){ this->value = s;}
		std::shared_ptr<std::string> Get() const {  return this->value;}
		
		void Add(ContainerParaType v)
		{ 
			std::for_each(v->cbegin(), v->cend(), [this](auto i){ this->Add(i); });
		}
		
		void Add(ValueType v)
		{ 
			if(this->parsedValues->size() > 0 && (*(this->parsedValues->end()-1))->Next() == nullptr)
				(*(this->parsedValues->end()-1))->setNext(v);
			
			this->ctr += v->ParseValue()->size();	
			v->setCounter(this->parsedValues->size());
			this->parsedValues->push_back(v); 
		}
		
		std::ostream& Display(std::ostream& out)
        {
			for(auto it = this->parsedValues->cbegin(); it != this->parsedValues->cend(); ++it)
				(*it)->Display(out);
			
			return out;
        }

	private:
		uint ctr = 0;
		std::unique_ptr<ContainerType> parsedValues = std::make_unique<ContainerType>();
		std::shared_ptr<std::string> value = std::make_shared<std::string>();
	};
	
	std::ostream& operator<<(std::ostream& strm, std::shared_ptr<ParserState> c)
	{
	        return c->Display(strm);
	}

}

#endif
