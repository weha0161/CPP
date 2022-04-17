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
			out<<this->ctr<<": "<<*(this->strValue)<<std::endl;
			
			return out;
        }
                
	protected:
		uint ctr;
		ParaType strValue;
		BasePtrType next;

		virtual ~ParsedValue()	{ /*Logger::Log()<<"Destructor"<<std::endl;*/ }
		ParsedValue& operator=(const ParsedValue&){ return *this;};
		ParsedValue(const ParsedValue& c){};
		ParsedValue(BasePtrType n = nullptr): next{n}{};
		ParsedValue(ParaType s,BasePtrType n = nullptr): next{n}, strValue{s}{	};
        
        static auto extractCommonAtoms(ParsedValue::ParaType val)
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
			
			return ret;
		}
	private:
		void setNext(BasePtrType ptr)
		{ 
			//~ Logger::Log()<<"Current: "<<*(this->strValue)<<"Next: "<<*(ptr->ParseValue())<<std::endl;
			this->next = ptr; 
		}
		
		void setCounter(uint c)	{ this->ctr = c; }
	};
	
	class ParsedInt: public ParsedValue
	{
		using PtrType = std::shared_ptr<ParsedInt>;
		inline static auto ZERO = T::char_<'0'>(); 
	public:
		ParsedInt(ParsedValue::ParaType val, ParsedValue::BasePtrType next = nullptr): vals(std::make_shared<std::vector<uint>>()), ParsedValue(val, next)
		{
			if(val->size() > 0)
			{
				for(auto  i = (val->size())-1;i > 0; --i)
					vals->push_back((uint)(val->at(i) - ZERO));

				vals->push_back((uint)(val->at(0) - ZERO));
				Logger::Log()<<"INT Res"<<(vals->size() ? *(vals->cend()-1)  : 0)<<"_"<<(vals->size() ? *(vals->cbegin())  : 0)<<std::endl;
				//~ std::stoll(*val);
			}
				
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
		ParsedPunct(ParsedValue::ParaType val, ParsedValue::BasePtrType next = nullptr): ParsedValue(val, next)
		{						
			Logger::Log()<<"VAL Constructor Point: "<<*val<<std::endl;
		}
		
		static auto Create(ParsedValue::ParaType p)
		{
			auto commons = extractCommonAtoms(p);
			ReturnType ret = std::make_shared<std::vector<PtrType>>();
			for(auto it = commons->cbegin(); it != commons->cend(); ++it)
				ret->push_back(SpecialAtomContainerType::Instance().Parse(*it));
			return ret;
		}
		
		std::string Value(){ return "Test"; }
	};
	
	template<typename T>
	class SpecializedPunctation: public ParsedPunct
	{
	public:
		using Base = ParsedPunct;
		SpecializedPunctation(ParsedValue::ParaType val, ParsedValue::BasePtrType next = nullptr): ParsedPunct(val, next){};
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
	
	template<typename T>
	struct Creator
	{
		//~ typename T::ContainerParaType operator()(typename T::ParaType p)
		static typename T::ContainerParaType Parse(typename T::ParaType p)
		{
			auto ret = std::make_shared<typename T::ContainerType>();
			ret->push_back(std::make_shared<T>(p));
			return ret;
		}
	};
	
	template<>
	struct Creator<ParsedPunct>
	{
		//~ typename ParsedPunct::ContainerParaType operator()(typename ParsedPunct::ParaType p)
		static typename ParsedPunct::ContainerParaType Parse(typename ParsedPunct::ParaType p)
		{
			auto ret = std::make_shared<typename ParsedPunct::ContainerType>();
			auto vals = ParsedPunct::Create(p);
			ret->insert(ret->end(), vals->begin(), vals->end());
			return ret;
		}
	};
}

#endif
