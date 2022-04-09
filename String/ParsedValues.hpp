#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <memory>
#include "../Wrapper/Wrapper.hpp"
#include "../Traits/Traits.h"
#include "../Typelist/Typelist.h"
#include "../Logger/Logger.hpp"

#ifndef PARSEDVALUES_HPP
#define PARSEDVALUES_HPP

namespace String_
{
	struct ParserState;
	
	class ParsedValue
	{
		friend class ParserState;
	public:
		using ParaType = std::shared_ptr<std::string>;
		using BasePtrType = std::shared_ptr<ParsedValue>;
		BasePtrType Next() { return this->next; };
		//~ static str Make
		
	protected:
		int ctr;
		ParaType strValue;
		BasePtrType next;

		virtual ~ParsedValue()	{ /*Logger::Log()<<"Destructor"<<std::endl;*/ }
		ParsedValue& operator=(const ParsedValue&){ return *this;};
		ParsedValue(const ParsedValue& c){};
		ParsedValue(BasePtrType n = nullptr): next{n}{};
		ParsedValue(ParaType s,BasePtrType n = nullptr): next{n}, strValue{s}{	};
	private:
		void setNext(BasePtrType ptr){ this->next = ptr; Logger::Log()<<"Add"<<std::endl; }
	};
	
	class ParsedInt: public ParsedValue
	{
		using PtrType = std::shared_ptr<ParsedInt>;
	public:
		ParsedInt(ParsedValue::ParaType val, ParsedValue::BasePtrType next = nullptr): ParsedValue(val, next)
		{
		}
		int Value(){ return 5; }
	};
	
	class ParsedWord: public ParsedValue
	{
		using PtrType = std::shared_ptr<ParsedWord>;
	public:
		ParsedWord(ParsedValue::ParaType val, ParsedValue::BasePtrType next = nullptr): ParsedValue(val, next)
		{
			Logger::Log()<<"VAL: "<<*val<<std::endl;
		}
		
		std::string Value(){ return "Test"; }
	};
}

#endif
