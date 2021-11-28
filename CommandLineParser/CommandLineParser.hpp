#include <string>
#include <iostream>

#ifndef COMMANDLINEPARSER_HPP
#define COMMANDLINEPARSER_HPP

namespace CommandLine
{
	class Parser
	{
		#include "ParserState.hpp"
		std::string s;
	public:
		Parser(){};
		virtual void event(char c)
		{ 
			std::cout<<c; 
			s += c;
		};
		virtual ~Parser(){};
	};
}

#endif
