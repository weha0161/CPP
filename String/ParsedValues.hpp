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
        
    class ParsedValue;
	using BasePtrType = std::shared_ptr<ParsedValue>;
    
    template<typename TVal, typename TParsed> struct IsImpl{ bool operator ()(TParsed bp){ Logger::Log()<<"T"<<std::endl; return false;}};
	
	class ParsedValue
	{
		friend class ParserState;
	public:
		using ParaType = std::shared_ptr<std::string>;
		using BasePtrType = std::shared_ptr<ParsedValue>;
		using ContainerType = std::vector<BasePtrType>;
		using ContainerParaType = std::shared_ptr<ContainerType>;
		
		BasePtrType Next() const { return this->next; };
		ParaType Value() const { return this->strValue;}
		
		virtual ContainerParaType Get(ParaType p) { return ContainerParaType(); } 
		
		std::ostream& Display(std::ostream& out)
        {
			out<<this->ctr<<": "<<*(this->strValue)<<std::endl;
			
			return out;
        }
        template<typename TVal>
        bool Is()
        {
			auto is = std::make_shared<IsImpl<TVal,BasePtrType>>();
			return (*is)(std::make_shared<ParsedValue>(*this));
		}
        
        char operator[](uint i) const{ return this->strValue->at(i); }
        size_t Size() const { return this->strValue->size(); }
                
		ParsedValue(const ParsedValue& c)
		{
			this->next = c.Next();
			this->strValue = c.Value();
		};
		virtual ~ParsedValue()	{ }
	protected:
		uint ctr;
		ParaType strValue;
		BasePtrType next;

		ParsedValue& operator=(const ParsedValue&){ return *this;};
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
			//~ Logger::Log()<<"Current: "<<*(this->strValue)<<"Next: "<<*(ptr->Value())<<std::endl;
			this->next = ptr; 
		}
		
		void setCounter(uint c)	{ this->ctr = c; }
	};
	
	
	class ParsedWord: public ParsedValue
	{
		using PtrType = std::shared_ptr<ParsedWord>;
	public:
		ParsedWord(ParsedValue::ParaType val, ParsedValue::BasePtrType next = nullptr): ParsedValue(val, next){	Logger::Log()<<"VAL Constructor Word: "<<*val<<std::endl;}
		
		std::string Cast(){ return "Test"; }
	};
	
	class ParsedBlank: public ParsedValue
	{
		using PtrType = std::shared_ptr<ParsedBlank>;
	public:
		ParsedBlank(ParsedValue::ParaType val, ParsedValue::BasePtrType next = nullptr): ParsedValue(val, next)	{	Logger::Log()<<"VAL Constructor Blank: "<<*val<<std::endl;	}
		
		std::string Value(){ return "Test"; }
	};
	
	template<typename T>
	class SpecializedPunctation;
	
	class ParsedPunct: public ParsedValue
	{
		using PtrType = std::shared_ptr<ParsedPunct>;
		using ReturnType = ReturnT<ParsedPunct>;
		using Types = Typelist<T::char_<'.'>,T::char_<','>,  T::char_<'!'>, T::char_<'"'>,T::char_<'#'>,T::char_<'$'>,T::char_<'%'>,T::char_<'&'>,T::char_<'\''>,T::char_<'<'>,
		T::char_<'('>,T::char_<')'>,T::char_<'*'>,T::char_<'+'>,T::char_<'-'>,T::char_<'/'>,T::char_<':'>,T::char_<';'>,T::char_<'<'>,T::char_<'='>,T::char_<'?'>,
		T::char_<'@'>,T::char_<'['>,T::char_<'\\'>,T::char_<']'>,T::char_<'^'>,T::char_<'_'>,T::char_<'`'>,T::char_<'{'>,T::char_<'|'>,T::char_<'}'>, T::char_<'~'>>;
		
		using SpecialAtomContainerType = SpecialAtomContainer<ParsedPunct, Types>::ContainerType;
	protected:
		std::function<bool(char)> isImpl;
	public:
		ParsedPunct(ParsedValue::ParaType val, std::function<bool(char)> isPara, ParsedValue::BasePtrType next = nullptr): isImpl(isPara),ParsedValue(val, next){	}
		
		static auto Create(ParsedValue::ParaType p)
		{
			auto commons = extractCommonAtoms(p);
			ReturnType ret = std::make_shared<std::vector<PtrType>>();
			for(auto it = commons->cbegin(); it != commons->cend(); ++it)
				ret->push_back(SpecialAtomContainerType::Instance().Parse(*it));
			return ret;
		}
		
		template<typename P>
		bool Is() { return this->isImpl(P::Value); }

		std::string Cast(){ return "Test"; }
	};
	
	class ParsedInt: public ParsedValue
	{
		using PtrType = std::shared_ptr<ParsedInt>;
		using ValuesType = std::shared_ptr<std::vector<uint>>;
		inline static auto ZERO = T::char_<'0'>(); 
		ValuesType vals;
	public:
		ParsedInt(ParsedValue::ParaType val, ParsedValue::BasePtrType next = nullptr): vals(std::make_shared<std::vector<uint>>()), ParsedValue(val, next)
		{
			if(val->size() > 0)
			{
				for(auto  i = (val->size())-1;i > 0; --i)
					vals->push_back((uint)(val->at(i) - ZERO));

				vals->push_back((uint)(val->at(0) - ZERO));
				//~ std::stoll(*val);
			}				
		}
		
		uint Cast(){ return 5; }
		ValuesType Values() { return this->vals; }
		
		//~ uint operator[](uint i)	const { return i >= this->vals->size() ? this->vals->at(i) : 0;	}
		
		bool operator==(const ParsedInt& pi)
		{
			if(this->vals->size() != pi.Size())
				return false;
				
			for(auto i = 0; i < this->vals->size(); ++i)
				if(this->strValue->at(i) != pi[i])
					return false;
					
			return true;
		}
	};
	
	class ParsedSpace: public ParsedValue
	{
		using PtrType = std::shared_ptr<ParsedSpace>;
	public:
		ParsedSpace(ParsedValue::ParaType val, ParsedValue::BasePtrType next = nullptr): ParsedValue(val, next)	{ Logger::Log()<<"VAL Constructor Spacce: "<<*val<<std::endl;	}
		
		std::string Cast(){ return "Test"; }
	};
	
	template<typename T>
	struct Creator
	{
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
		static typename ParsedPunct::ContainerParaType Parse(typename ParsedPunct::ParaType p)
		{
			auto ret = std::make_shared<typename ParsedPunct::ContainerType>();
			auto vals = ParsedPunct::Create(p);
			ret->insert(ret->end(), vals->begin(), vals->end());
			return ret;
		}
	};
	
	template<> struct IsImpl<uint,ParsedValue::BasePtrType>{ bool operator ()(ParsedValue::BasePtrType bp){ Logger::Log()<<"UINT"<<std::endl; return true;}	};
	template<> struct IsImpl<double, ParsedValue::BasePtrType>
	{ 
		bool operator ()(ParsedValue::BasePtrType bp)
		{ 
			if(bp->Next() == nullptr) return false;
			if(bp->Next()->Next() == nullptr) return false;
			if(!std::dynamic_pointer_cast<ParsedInt>(bp->Next()->Next())) return false;

			auto pp = std::dynamic_pointer_cast<ParsedPunct>(bp->Next());
			if(!pp) return false;
			
			return pp->template Is<T::char_<','>>();
		}	
	};
        
}

#endif
