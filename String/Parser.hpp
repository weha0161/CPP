#include <string>
#include <cstring>
#include <iostream>
#include <vector>
// #include <strstream>
#include "../Wrapper/Wrapper.hpp"
#include "../Traits/Traits.h"
#include "../Typelist/Typelist.h"

#ifndef PARSE_HPP
#define PARSE_HPP

namespace String_
{
	class Exemplar{};
	class Atom
	{
	protected:
		static Atom* list;
		static char err;
		Atom* next;
	public:
		Atom(Exemplar){ next = list; list = this; }
		virtual Atom* make(std::string &s)
		{
			Atom* ret = 0;
			extern Atom* atom;
			for(Atom* a = list; a; a = a->next)
			{
				if( a != atom)
					if(ret = a->make(s)) break;
			}
			
			return ret;
		}
		
		Atom(): next(0) {}
		virtual ~Atom() {}
		virtual unsigned long Value() { return 0;}
		virtual operator char() { return 0;}
		virtual std::string Name() { return std::string("Error");}
	};
	
	Atom* Atom::list = 0;
	char Atom::err = 0;
	static Atom AtomInstance = Atom(Exemplar());
	Atom* atom = &AtomInstance;
	
	class Parser
	{
		
	public:
		Parser()
		{
			
		};
				
		void Parse()
		{
		
		}
		
		virtual ~Parser(){};
	};
	
}

#endif
