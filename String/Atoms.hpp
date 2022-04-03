#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <memory>
#include "ParserState.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../Traits/Traits.h"
#include "../Typelist/Typelist.h"
#include "../Logger/Logger.hpp"

#ifndef ATOMS_HPP
#define ATOMS_HPP

namespace String_
{
	template<typename>
	class Atom;
	
	template<typename T>
	class AtomBase
	{
	public:
		using State = ParserState;
		using Base = AtomBase<T>;
		using ValueType = T;
		using Type = Atom<T>;
		
		static Type& Instance()
		{
			static Type instance;
			return instance;
		}
		
		bool make(std::shared_ptr<ParserState> state)
		{
			state->Increment();
			Logger::Log()<<"Atom: "<<state->CurrentVal()<<std::endl;
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
	
	template<typename T>
	class Atom: public AtomBase<T>
	{
		friend class AtomBase<T>;
	protected:
		bool Is(){ return true; }
		Atom() = default;
	};
	
	template<>
	class Atom<int>: public AtomBase<int>
	{
		friend class AtomBase<int>;
	public:

	protected:
		bool Is(){ return true; }
		Atom<int>() = default;
	};
	
	template<>
	class Atom<std::string>: public AtomBase<std::string>
	{
		friend class AtomBase<std::string>;
	public:
	protected:
		bool Is(){ return true; }
		Atom<std::string>() = default;
	};	
}

#endif
