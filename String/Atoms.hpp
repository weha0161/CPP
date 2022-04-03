#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <memory>
#include "../Wrapper/Wrapper.hpp"
#include "../Traits/Traits.h"
#include "../Typelist/Typelist.h"
#include "../Logger/Logger.hpp"

#ifndef ATOMS_HPP
#define ATOMS_HPP

namespace String_
{
	template<typename,typename>
	class Atom;
	
	template<typename T,typename ParserState>
	class AtomBase
	{
	public:
		using State = ParserState;
		using Base = AtomBase<T, ParserState>;
		using ValueType = T;
		using Type = Atom<T,ParserState>;
		
		static Type& Instance()
		{
			static Type instance;
			return instance;
		}
		
		
		bool make(std::shared_ptr<ParserState> state)
		{
			Logger::Log()<<"Atom: "<<state->Counter()<<*state->Get()<<std::endl;
			if(Type::Instance().Is())
				return false;
// 				return Type::Parse(s);
			return true;
		}
		
		std::shared_ptr<ParserState> state;
	protected:
		inline static std::shared_ptr<T> values = std::make_shared<T>;

		~AtomBase()	{ /*Logger::Log()<<"Destructor"<<std::endl;*/ }
		AtomBase& operator=(const AtomBase&){};
		AtomBase(const AtomBase& c){};
		AtomBase(){};
		Type& operator=(const Type&){};
	};
	
	template<typename T, typename ParserState>
	class Atom: public AtomBase<T,ParserState>
	{
		friend class AtomBase<T,ParserState>;
	protected:
		bool Is(){ return true; }
		Atom() = default;
	};
	
	template<typename ParserState>
	class Atom<int, ParserState>: public AtomBase<int,ParserState>
	{
		friend class AtomBase<int,ParserState>;
	public:

	protected:
		bool Is(){ return true; }
		Atom<int,ParserState>() = default;
	};
	
	template<typename ParserState>
	class Atom<std::string, ParserState>: public AtomBase<std::string,ParserState>
	{
		friend class AtomBase<std::string,ParserState>;
	public:
	protected:
		bool Is(){ return true; }
		Atom<std::string,ParserState>() = default;
	};	
}

#endif
