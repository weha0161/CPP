#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <memory>
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
		Iterator Current() { return (this->value->cbegin() + ctr);}
		Iterator End() { return this->value->cend();}
		Iterator Begin() { return this->value->cbegin();}
		char CurrentVal() { return *(this->value->cbegin() + ctr);}
		void Increment() { ++ctr; }
		void Set(const std::string& s){ ParserState::value = std::make_shared<std::string>(s); ++ctr;}
		std::shared_ptr<std::string> Get() const {  return ParserState::value;}
		uint Counter(){ return ParserState::ctr; }
	private:
		inline static uint ctr = 0;
		inline static std::shared_ptr<std::string> value = std::make_shared<std::string>();
	};
}

#endif
