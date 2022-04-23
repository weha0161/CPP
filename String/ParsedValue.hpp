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

#ifndef PARSEDVALUE_HPP
#define PARSEDVALUE_HPP

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
}

#endif
