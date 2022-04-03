#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <memory>
#include "AtomContainer.hpp"
#include "Atoms.hpp"
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
		struct ParserState
		{
			void Set(const std::string& s){ ParserState::value = std::make_shared<std::string>(s); ++ctr;}
			std::shared_ptr<std::string> Get() const {  return ParserState::value;}
			uint Counter(){ return ParserState::ctr; }
		private:
			inline static uint ctr = 0;
			inline static std::shared_ptr<std::string> value = std::make_shared<std::string>();
		};
	public:
		using Types = Typelist<Atom<int, ParserState>,Atom<uint, ParserState>,Atom<double, ParserState>,Atom<std::string, ParserState>,Atom<char, ParserState>>;
		using AtomConatinerType = AtomContainer<Types>::ContainerType;
		
		Parser(): state(std::make_shared<ParserState>())
		{
			this->state->Set("1");
			AtomConatinerType::Instance().make(this->state);
			this->state->Set("2");
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
