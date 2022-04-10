#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <memory>
#include "AtomContainer.hpp"
#include "Atoms.hpp"
#include "ParserState.hpp"
#include "ParsedValues.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../Traits/Traits.h"
#include "../Typelist/Typelist.h"
#include "../Logger/Logger.hpp"

#ifndef ATOMPASSINGPARSER_HPP
#define ATOMPASSINGPARSER_HPP

namespace String_
{
	class Parser
	{	
	public:
		using Types = Typelist<Atom<ParsedPunct>, Atom<ParsedBlank>, Atom<ParsedSpace>,Atom<uint>,Atom<std::string>,Atom<char>>;
		using AtomConatinerType = AtomContainer<Types>::ContainerType;
		
		Parser(): state(std::make_shared<ParserState>())
		{
		};
				
		void Parse(std::shared_ptr<std::string> sp)
		{
			this->state->Set(sp);
			while(this->state->Current()!= this->state->End())
				AtomConatinerType::Instance().make(this->state);
			
			this->state->Display(std::cout);
		}
		void Parse(const std::string& s)
		{
			this->Parse(std::make_shared<std::string>(s));
		}
		
		virtual ~Parser(){};
	private:
		
		
		std::shared_ptr<ParserState> state;
	};
	
}

#endif
