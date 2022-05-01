#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <memory>
#include "SpecialAtomsContainer.hpp"
#include "QuantityContainer.hpp"
#include "ParsedValue.hpp"
#include "../Quantity/Quantity.h"
#include "../Common/Number.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../Traits/Traits.h"
#include "../Typelist/Typelist.h"
#include "../Logger/Logger.hpp"

#ifndef PARSEDVALUES_HPP
#define PARSEDVALUES_HPP

namespace String_
{
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
	
	class ParsedNumber: public ParsedValue
	{
		using PtrType = std::shared_ptr<ParsedNumber>;
		using ValuesType = std::shared_ptr<std::vector<uint>>;
		inline static auto ZERO = T::char_<'0'>(); 
		ValuesType vals;
		
		using Types = Typelist<Quantity<Sum>,Quantity<Length>,Quantity<Mass>,Quantity<Time>,Quantity<Current>,Quantity<Temperature>, Quantity<Volume>,Quantity<Work>,Quantity<Area>,Quantity<Scalar>>;
		
		
	public:
		using QuantityContainerType = QuantityContainer<Types>::ContainerType;
		ParsedNumber(ParsedValue::ParaType val, ParsedValue::BasePtrType next = nullptr): vals(std::make_shared<std::vector<uint>>()), ParsedValue(val, next)
		{
			if(val->size() > 0)
			{
				for(auto  i = (val->size())-1;i > 0; --i)
					vals->push_back((uint)(val->at(i) - ZERO));

				vals->push_back((uint)(val->at(0) - ZERO));
				//~ std::stoll(*val);
			}				
		}
		
		decltype(auto) GetQuantityUnit(){ return QuantityContainerType::Instance().Get(this->Cast<int>(),this->next->Value());	}
		
		template<typename Q>
		static decltype(auto) Get()
		{
			return QuantityContainerType::Instance().Get<Q>();
		}
		
		decltype(auto) AsQuantity()
		{
			Logger::Log()<<"GET Quanr: "<<QuantityContainerType::Instance().Get(this->Cast<int>(),this->next->Value())<<std::endl;
			return QuantityContainerType::Instance().Add(this->Cast<int>(),this->next->Value());
		}
		
		ValuesType Values() { return this->vals; }
		
		//~ uint operator[](uint i)	const { return i >= this->vals->size() ? this->vals->at(i) : 0;	}
		
		bool operator==(const ParsedNumber& pi)
		{
			if(this->Size() != pi.Size())
				return false;
				
			for(auto i = 0; i < this->Size(); ++i)
				if(this->strValue->at(i) != pi[i])
					return false;
					
			return true;
		}
		
		bool operator<(const ParsedNumber& pi)
		{
			if(this->Size() >= pi.Size())
				return false;
				
			for(auto i = 0; i < this->vals->size(); ++i)
				if(this->strValue->at(i) != pi[i])
					return false;
					
			return true;
		}
		
		static std::ostream& Display(std::ostream& os) { return QuantityContainerType::Display(os);	}
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
			if(!std::dynamic_pointer_cast<ParsedNumber>(bp->Next()->Next())) return false;

			auto pp = std::dynamic_pointer_cast<ParsedPunct>(bp->Next());
			if(!pp) return false;
			
			return pp->template Is<T::char_<','>>();
		}	
	};
        
}

#endif
