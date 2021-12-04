#include <string>
#include <iostream>
#include "../Logger/Logger.hpp"

#ifndef PARSERSTATE_HPP
#define PARSERSTATE_HPP

namespace CommandLine
{
	class ParserState
	{
		const std::string ID;
	protected:
		ParserState(std::string id): ID(id){}
	public:
		virtual ParserState* Event( char c) 
		{ 
			Logger::Log()<<ID<<"\t"<<c<<" = \t"<<(int)c<<std::endl;
			return this; 
		}
		static ParserState* Reset() { return Incomplete; }
		virtual ~ParserState() {}
		static ParserState *Invalid, *Valid, *Incomplete;		
	};
	
	class InvalidState: public ParserState
	{
	public:
		InvalidState(): ParserState("Invalid"){};
		virtual ParserState* Event(char c) 
		{ 
			ParserState::Event(c);
			return this; 			
		}
	};
	
	class ValidState: public ParserState
	{
	public:
		ValidState(): ParserState("Valid"){};
		virtual ParserState* Event(char c)
		{ 
			ParserState::Event(c);
			return this; 			
		}
	};
	
	class IncompleteState: public ParserState
	{
	public:
		IncompleteState(): ParserState("Incomplete"){};
		virtual ParserState* Event(char c) 
		{ 
			ParserState::Event(c);
			return this; 			
		}
	};
	
	
	ParserState* ParserState::Invalid = new InvalidState; 
	ParserState* ParserState::Valid = new ValidState; 
	ParserState* ParserState::Incomplete = new IncompleteState; 
}

#endif
