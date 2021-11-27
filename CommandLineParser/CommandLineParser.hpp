#include <string>
#include <iostream>

#ifndef COMMANDLINEPARSER_HPP
#define COMMANDLINEPARSER_HPP

namespace CommandLine
{
	class Parser
	{
		#include "ParserState.hpp"
	public:
		Parser(){};
		virtual void event(char c){ std::cout<<c; };
		virtual ~Parser(){};
	};
}

#endif
