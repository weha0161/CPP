#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <memory>

#include "SpecialAtomsContainer.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../Traits/Traits.h"
#include "../Typelist/Typelist.h"
#include "../Logger/Logger.hpp"

#ifndef PARSEDVALUES_HPP
#define PARSEDVALUES_HPP

namespace String_
{
	struct ParserState;
	
	template<typename T>
	using ReturnT = std::shared_ptr<std::vector<std::shared_ptr<T>>>;
	
	class ParsedValue
	{
		friend class ParserState;
	public:
		using ParaType = std::shared_ptr<std::string>;
		using BasePtrType = std::shared_ptr<ParsedValue>;
		using ContainerType = std::vector<BasePtrType>;
		using ContainerParaType = std::shared_ptr<ContainerType>;
		
		BasePtrType Next() { return this->next; };

		ParaType ParseValue(){ return this->strValue;}
		
		virtual ContainerParaType Get(ParaType p) { return ContainerParaType(); } 
		
		std::ostream& Display(std::ostream& out)
        {
			out<<*(this->strValue)<<std::endl;
			
			return out;
        }
        
	protected:
		int ctr;
		ParaType strValue;
		BasePtrType next;

		virtual ~ParsedValue()	{ /*Logger::Log()<<"Destructor"<<std::endl;*/ }
		ParsedValue& operator=(const ParsedValue&){ return *this;};
		ParsedValue(const ParsedValue& c){};
		ParsedValue(BasePtrType n = nullptr): next{n}{};
		ParsedValue(ParaType s,BasePtrType n = nullptr): next{n}, strValue{s}{	};
        
        void P()
        {
			uint i = 1;
			auto it = strValue->cend();
			for(; (it-i) != strValue->cbegin();++i)
				Logger::Log()<<"VAL Constructor Word: "<<*(it-i)<<std::endl;
			
			Logger::Log()<<"VAL Constructor Word: "<<*(it-i)<<std::endl;
		}
	private:
		void setNext(BasePtrType ptr){ this->next = ptr; }
	};
	
	class ParsedInt: public ParsedValue
	{
		using PtrType = std::shared_ptr<ParsedInt>;
	public:
		ParsedInt(ParsedValue::ParaType val, ParsedValue::BasePtrType next = nullptr): vals(std::make_shared<std::vector<uint>>()), ParsedValue(val, next)
		{
			Logger::Log()<<"VAL Constructor Int: "<<*val<<std::endl;
		}
		uint Value(){ return 5; }
		std::shared_ptr<std::vector<uint>> vals;
	};
	
	class ParsedWord: public ParsedValue
	{
		using PtrType = std::shared_ptr<ParsedWord>;
	public:
		ParsedWord(ParsedValue::ParaType val, ParsedValue::BasePtrType next = nullptr): ParsedValue(val, next)
		{			
			Logger::Log()<<"VAL Constructor Word: "<<*val<<std::endl;
		}
		
		std::string Value(){ return "Test"; }
	};
	
	template<typename T>
	class SpecializedPunctation;
	
	class ParsedPunct: public ParsedValue
	{
		using PtrType = std::shared_ptr<ParsedPunct>;
		using ReturnType = ReturnT<ParsedPunct>;
		using Types = Typelist<T::char_<'.'>,T::char_<','>>;
		using SpecialAtomContainerType = SpecialAtomContainer<ParsedPunct, SpecializedPunctation, Types>::ContainerType;
	public:
		ParsedPunct(){};
		ParsedPunct(ParsedValue::ParaType val, ParsedValue::BasePtrType next = nullptr): ParsedValue(val, next)
		{
			
			auto ret = std::make_shared<std::vector<ParaType>>();
			auto first = *(val->cbegin());
			auto temp = std::string(val->cbegin(),val->cbegin()+1);
			
			for(auto it = val->cbegin() + 1; it != val->cend();++it)
			{
				if(temp.at(0) == *it)
					temp +=*it;
				else
				{
					ret->push_back(std::make_shared<std::string>(temp.cbegin(), temp.cend()));
					temp = std::string(it,it+1);
				}
			}

			ret->push_back(std::make_shared<std::string>(temp.cbegin(), temp.cend()));
			
			for(auto it = ret->cbegin(); it != ret->cend();++it)
			{
				Logger::Log()<<"VALS "<<**it<<std::endl;
				
			}
			
			SpecialAtomContainerType::Instance().Parse(val);
			Logger::Log()<<"VAL Constructor Point: "<<*val<<std::endl;
		}
		
		std::string Value(){ return "Test"; }
	};
	
	template<typename T>
	class SpecializedPunctation: public ParsedPunct
	{
	public:
		using Base = ParsedPunct;
		SpecializedPunctation(): ParsedPunct(){};
	};
	
	class ParsedBlank: public ParsedValue
	{
		using PtrType = std::shared_ptr<ParsedBlank>;
	public:
		ParsedBlank(ParsedValue::ParaType val, ParsedValue::BasePtrType next = nullptr): ParsedValue(val, next)
		{
			Logger::Log()<<"VAL Constructor Blank: "<<*val<<std::endl;
		}
		
		std::string Value(){ return "Test"; }
	};
	
	class ParsedSpace: public ParsedValue
	{
		using PtrType = std::shared_ptr<ParsedSpace>;
	public:
		ParsedSpace(ParsedValue::ParaType val, ParsedValue::BasePtrType next = nullptr): ParsedValue(val, next)
		{
			Logger::Log()<<"VAL Constructor Spacce: "<<*val<<std::endl;
		}
		
		std::string Value(){ return "Test"; }
	};
}

#endif
