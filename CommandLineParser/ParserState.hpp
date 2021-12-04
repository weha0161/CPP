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
		static ParserState* Reset() { return incomplete; }
		virtual ~ParserState() {}
// 	protected:
		static ParserState *invalid, *valid, *incomplete;		
	};
	
	class Invalid: public ParserState
	{
	public:
		Invalid(): ParserState("Invalid"){};
		virtual ParserState* Event(char c) 
		{ 
			ParserState::Event(c);
			return this; 			
		}
	};
	
	class Valid: public ParserState
	{
	public:
		Valid(): ParserState("Valid"){};
		virtual ParserState* Event(char c)
		{ 
			ParserState::Event(c);
			return this; 			
		}
	};
	
	class Incomplete: public ParserState
	{
	public:
		Incomplete(): ParserState("Incomplete"){};
		virtual ParserState* Event(char c) 
		{ 
			ParserState::Event(c);
			return this; 			
		}
	};
	
	
	ParserState* ParserState::invalid = new Invalid; 
	ParserState* ParserState::valid = new Valid; 
	ParserState* ParserState::incomplete = new Incomplete; 
}

#endif
