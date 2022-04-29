#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
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
	template<typename QuantityT>
	using QuantityMap = std::map<uint, QuantityT> ;
	
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
		const std::string Get(ValueType v,ParaType p)
		{
			Logger::Log()<<"Unit "<<Head::UnitType::Sign()<<std::endl;
			return Head::UnitType::Sign();
		}
		
		template<typename ValueType>
		void Add(ValueType v,ParaType p)
		{
			this->quantities->insert(std::pair<uint,Head>(0,Head(v) ));
			Logger::Log()<<"Unit "<<Head::UnitType::Sign()<<std::endl;
		}
		
		
		static QuantityContainer& Instance()
		{
			static QuantityContainer instance;
			return instance;
		}	
	private:
		inline static std::unique_ptr<QuantityMap<Head>> quantities = std::make_unique<QuantityMap<Head>>();
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
		const std::string Get(ValueType v,ParaType p)
		{
			  if(*p==Head::UnitType::Sign())
					return Head::UnitType::Sign();

              return Base::Get(v,p);
		}
		
		template<typename ValueType>
		void Add(ValueType v,ParaType p)
		{
			if(*p==Head::UnitType::Sign())
			{
				this->quantities->insert(std::pair<uint,Head>(0,Head(v) ));
				Logger::Log()<<"RETURN"<<" "<<this->quantities->cbegin()->second<<std::endl;
				return;
			}
				
			Base::Add(v,p);
		}

		static QuantityContainer& Instance()
		{
			static QuantityContainer instance;
			return instance;
		}	
	private:
		inline static std::unique_ptr<QuantityMap<Head>> quantities = std::make_unique<QuantityMap<Head>>();
	};

	template<typename Head, typename... Tail>
	std::ostream& operator<<(std::ostream& strm, const QuantityContainer<Head,Tail...> c)
	{
		return c.Display(strm);
	}
}
#endif
