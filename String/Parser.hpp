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
	
	class Number: public Atom
	{
	public:
		Number(Exemplar a): Atom(a) { }
		Number(std::string& s): Atom()
		{
			sum = 0;
			for(int i = 0; s[i] >= '0' && s[i] <= '9'; ++i)
			{
				sum = (sum * 10) + s[i] - '0';
				char c = s[i];
				err = !(ispunct(c) || isspace(c));
			}			
		}
		
		virtual Atom* make(std::string &s)
		{
			Atom* ret = new Number(s);
			if(err)
			{
				delete ret;
				ret = 0;
			}
			
			return ret;
		}
		
		Number(Number& n): sum(n.Value()) {}
		virtual ~Number() {}
		virtual unsigned long Value() const { return sum; }
	private:
		unsigned long sum;
	};
	
	static Number NumberInstance = Number(Exemplar());
	
	class Word: public Atom
	{
	public:
		Word(Exemplar a): Atom(a) { }
		Word(std::string& s): Atom()
		{
			for(int i = 0; s[i] >= 'a' && s[i] <= 'z'; ++i)
			{
				n = n + s[i];
				if (!isalnum(s[i])) err++;
			}			
		}
		
		virtual Atom* make(std::string &s)
		{
			Atom* ret = new Word(s);
			if(err)
			{
				delete ret;
				ret = 0;
			}
			
			return ret;
		}
		
		Word(Word& n): n(n.Name()) {}
		std::string Name() { return n; }
		virtual ~Word() {}
	private:
		std::string n;
	};
	
	static Word NameInstance = Word(Exemplar());
	
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
