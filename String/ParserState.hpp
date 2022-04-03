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
		void Set(const std::string& s){ ParserState::value = std::make_shared<std::string>(s); ++ctr;}
		std::shared_ptr<std::string> Get() const {  return ParserState::value;}
		uint Counter(){ return ParserState::ctr; }
	private:
		inline static uint ctr = 0;
		inline static std::shared_ptr<std::string> value = std::make_shared<std::string>();
	};
}

#endif
