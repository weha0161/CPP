#include <string>
#include <iostream>
#include "ParserState.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../Logger/Logger.hpp"


#ifndef COMMANDLINEPARSER_HPP
#define COMMANDLINEPARSER_HPP

namespace CommandLine
{
	class Parser
	{
		using Exit =  T::char_<27>;
		std::string extractedValue;
		char currentValue;
		ParserState* state = ParserState::Incomplete; 
	public:
		Parser()
		{
			
		};
		virtual void Event()
		{ 
			Logger::Log()<<"PARSER EVENT: "<<this->currentValue<<" / "<<(int)this->currentValue<<std::endl;
			if(this->currentValue == Exit::Value)
				state = ParserState::Exit->Event(this->currentValue);
			if(this->extractedValue == "TEST")
				state = state->Event(this->currentValue);
			else
			{
				this->extractedValue += (this->currentValue);
				state = state->Event(this->currentValue);
			}
		};
		
		void Parse()
		{
			do
			{
				this->Event();
			}
			while((this->currentValue = std::cin.get()) != Exit::Value);
		}
		
		virtual ~Parser(){};
	};
}

#endif
