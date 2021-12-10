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
		using Enter =  T::char_<10>;
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
			Logger::Log()<<"extractedValue: "<<this->extractedValue<<std::endl;
			
			if(this->currentValue < 0 || this->currentValue == Enter::Value)
				return;
			
			if(this->currentValue == Exit::Value)
				state = ParserState::Exit->Event(this->currentValue);
			
			
			if(this->extractedValue == "TEST")
				state = ParserState::Valid->Event(this->currentValue);
			else
			{
				this->extractedValue += (this->currentValue);
				std::vector<std::string> vals { "tes", "tess" };
				state = state->Event(this->currentValue, vals.cbegin(), vals.cend());
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
