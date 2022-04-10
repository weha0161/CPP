#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <memory>
#include "ParserState.hpp"
#include "ParsedValues.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../Traits/Traits.h"
#include "../Typelist/Typelist.h"
#include "../Logger/Logger.hpp"

#ifndef ATOMS_HPP
#define ATOMS_HPP

namespace String_
{
	using It = std::string::const_iterator;

	template<typename>
	class Atom;
	
	template<typename T, typename ParsedT>
	class AtomBase
	{
	public:
		using State = ParserState;
		using StatePara = std::shared_ptr<ParserState>;
		using Base = AtomBase<T,ParsedT>;
		using ValueType = T;
		using ParsedValueType = ParsedT;
		using Type = Atom<T>;
		using ContainerType = std::vector<T>;
		
		static Type& Instance()
		{
			static Type instance;
			return instance;
		}
		
		bool make(StatePara state)
		{
			Logger::Log()<<"Atom: "<<*(state->Current())<<std::endl;
			if(!Type::Instance().Is(state->Current()))
				return false;
			
			Logger::Log()<<"Current 1: "<<*(state->Current())<<std::endl;
			uint ctr = 0;
			while(Type::Instance().Is(state->Current() + ctr)) 
				++ctr;
				
			Logger::Log()<<"Current: "<<*(state->Current() + ctr-1)<<std::endl;
			state->Add(std::make_shared<ParsedT>(std::make_shared<std::string>(state->Current(),state->Current() + ctr)));
			return true;
		}
		
		std::shared_ptr<ParserState> state;
	protected:
		inline static std::shared_ptr<ContainerType> values = std::make_shared<ContainerType>();

		~AtomBase()	{ /*Logger::Log()<<"Destructor"<<std::endl;*/ }
		AtomBase& operator=(const AtomBase&){};
		AtomBase(const AtomBase& c){};
		AtomBase(){};
		Type& operator=(const Type&){};
	};
	
	template<typename T>
	class Atom: public AtomBase<T,ParsedWord>
	{
		friend class AtomBase<T,ParsedWord>;
	public:
		bool Is(It it){ return false; }
	};
	
	template<>
	class Atom<int>: public AtomBase<int, ParsedInt>
	{
		friend class AtomBase<int,ParsedInt>;
	public:
		bool Is(It it){ return std::isdigit(*it); }
	};
	
	template<>
	class Atom<std::string>: public AtomBase<std::string, ParsedWord>
	{
		friend class AtomBase<std::string,ParsedWord>;
	public:
		bool Is(It it){ return std::isalpha(*it); }
	};	
}

#endif
