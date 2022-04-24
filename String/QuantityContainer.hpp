#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <filesystem>
#include "../Logger/Logger.hpp"
#include "../Typelist/Typelist.h"

#ifndef QUANTITYCONTAINER_HPP
#define QUANTITYCONTAINER_HPP

//---------------------------------------------------------------------------------------------------QuantityContainer----------------------------------------------------------------------------------------
namespace String_
{
	using ParaType = std::shared_ptr<std::string>;
	
	template<typename List>
	class QuantityContainer{};

	template<typename Head>
	class QuantityContainer<Typelist<Head>>
	{
	public:
		using Type = Head;
		using BaseType = Head;
		using SpecialAtomTypes = Typelist<Head>;
		using ContainerType = QuantityContainer<Typelist<Head>>;
	protected:
		QuantityContainer() 
		{ 
			Logger::Log<Info>()<<Head::UnitType::Sign()<<std::endl; 
		};
	public:
		static std::ostream& Display(std::ostream& os) 
		{
			return Type::Display(os);
		}
		
		template<typename ValueType>
		Head Get(ValueType v,ParaType p)
		{
			Logger::Log()<<"Unit "<<Head::UnitType::Sign()<<std::endl;
			return Head(0.0);
		}
		
		static QuantityContainer& Instance()
		{
			static QuantityContainer instance;
			return instance;
		}	
	};

	template<typename Head, typename... Tail>
	class QuantityContainer<Typelist<Head,Tail...>>: public QuantityContainer<Typelist<Tail...>>
	{
	public:
		using Type = Head;
		using SpecialAtomTypes = Typelist<Head,Tail...>;
		using ContainerType = QuantityContainer<Typelist<Head,Tail...>>;
		using Base = QuantityContainer<Typelist<Tail...>>;
	protected:
		QuantityContainer() { 
			Logger::Log<Info>()<<Head::UnitType::Sign()<<std::endl; 
		};
	public:
		static std::ostream& Display(std::ostream& os) 
		{
			return Base::Display(Type::Display(os));		
		}
		
		template<typename ValueType>
		Head Get(ValueType v,ParaType p)
		{
			Logger::Log()<<"Unit "<<Head::UnitType::Sign()<<" == "<<*p<<std::endl;
			if(*p==Head::UnitType::Sign())
			{
				//~ Logger::Log()<<"IS1: "<<(p->at(0)==Head::Value)<<" "<<Head::Value<<" "<<p->at(0)<<std::endl;
				return Head(v);
			}
				
			return Base::Get(v,p);
		}

		static QuantityContainer& Instance()
		{
			static QuantityContainer instance;
			return instance;
		}	
	};

	template<typename Head, typename... Tail>
	std::ostream& operator<<(std::ostream& strm, const QuantityContainer<Head,Tail...> c)
	{
		return c.Display(strm);
	}
}
#endif
