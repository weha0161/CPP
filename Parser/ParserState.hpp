#include <string>
#include <iostream>
#include "../Logger/Logger.hpp"

#ifndef PARSERSTATE_HPP
#define PARSERSTATE_HPP

namespace CommandLine
{
	class ParserState
	{
	protected:
		const std::string ID;
		ParserState(std::string id): ID(id){}
		using Iterator = std::vector<std::string>::const_iterator;
	public:
		
		virtual ParserState* Event(char c) 
		{ 
			Logger::Log()<<ID<<"\t"<<c<<" = \t"<<(int)c<<std::endl;
			return this; 
		}
		
		virtual ParserState* Event(char c, Iterator begin, Iterator end) 
		{ 
			Logger::Log()<<ID<<"\t"<<c<<" = \t"<<(int)c<<std::endl;
			return this; 
		}
		
		static ParserState* Reset() { return Incomplete; }
		virtual ~ParserState() {}
		static ParserState *Invalid, *Valid, *Incomplete, *Exit;		
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
		
		virtual ParserState* Event(char c, Iterator begin, Iterator end) 
		{ 
			ParserState::Event(c);
			
			std::cout<<"No match, did you mean:\t";
			
			for(Iterator it = begin; it != end; ++it)
			{
				std::cout<<*it<<"\t";
			}
			
			std::cout<<std::endl;
			
			return this; 
		}
	};
	
	class ExitState: public ParserState
	{
	public:
		ExitState(): ParserState("Exit"){};
		virtual ParserState* Event(char c) 
		{ 
			ParserState::Event(c);
			return this; 			
		}
	};
	
	
	ParserState* ParserState::Invalid = new InvalidState; 
	ParserState* ParserState::Valid = new ValidState; 
	ParserState* ParserState::Incomplete = new IncompleteState; 
	ParserState* ParserState::Exit = new ExitState; 
}

#endif
