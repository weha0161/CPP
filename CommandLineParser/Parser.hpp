#include <string>
#include <iostream>
#include "ParserState.hpp"

#ifndef COMMANDLINEPARSER_HPP
#define COMMANDLINEPARSER_HPP

namespace CommandLine
{
	class Parser
	{
		std::string s;
		ParserState* state = ParserState::Incomplete; 
	public:
		Parser()
		{
			
		};
		virtual void Event(char c)
		{ 
			if(s == "TEST")
				state = state->Event(c);
			else
			{
				s += c;
				state = state->Event(c);
			}
		};
		virtual ~Parser(){};
	};
}

#endif
