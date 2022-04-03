#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <memory>
#include "AtomContainer.hpp"
#include "Atoms.hpp"
#include "ParserState.hpp"
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
		using Types = Typelist<Atom<int>,Atom<uint>,Atom<double>,Atom<std::string>,Atom<char>>;
		using AtomConatinerType = AtomContainer<Types>::ContainerType;
		
		Parser(): state(std::make_shared<ParserState>())
		{
			this->state->Set("TEST1");
			AtomConatinerType::Instance().make(this->state);
			this->state->Set("TEST2");
			AtomConatinerType::Instance().make(this->state);
// 			
		};
				
		void Parse(std::string& s)
		{
// 			Atom* a = nullptr;
			if(s.length())
				{}
		}
		
		virtual ~Parser(){};
	private:
		
		
		std::shared_ptr<ParserState> state;
	};
	
}

#endif
