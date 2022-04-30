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
		using MapType = std::shared_ptr<QuantityMap<Head>>;
	protected:
		QuantityContainer() { Logger::Log<Info>()<<index<<": "<<"Quantity "<<Head::UnitType::Name<<std::endl; };
		inline static uint index = 0;
	public:
		static std::ostream& Display(std::ostream& os) 
		{
			for(auto it = quantities->cbegin(); it != quantities->cend(); ++it)
				os<<it->second<<std::endl;
			return os;
		}
		
		template<typename ValueType>
		const std::string Get(ValueType v,ParaType p)
		{
			Logger::Log()<<"Unit "<<Head::UnitType::Name<<std::endl;
			return Head::UnitType::Name;
		}
		
		template<typename Q>
		size_t Size()
		{
			  if(std::is_same<Head,Q>::value)
					return quantities->size();

             throw "Unknown Type";
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
		inline static std::shared_ptr<QuantityMap<Head>> quantities = std::make_shared<QuantityMap<Head>>();
	};

	template<typename Head, typename... Tail>
	class QuantityContainer<Typelist<Head,Tail...>>: public QuantityContainer<Typelist<Tail...>>
	{
	public:
		using Type = Head;
		using SpecialAtomTypes = Typelist<Head,Tail...>;
		using ContainerType = QuantityContainer<Typelist<Head,Tail...>>;
		using Base = QuantityContainer<Typelist<Tail...>>;
		using MapType = std::shared_ptr<QuantityMap<Head>>;
	protected:
		QuantityContainer() { Logger::Log<Info>()<<index<<": "<<"Quantity "<<Head::UnitType::Name<<std::endl; };
		inline static uint index = Base::index + 1; 
	public:
		static std::ostream& Display(std::ostream& os) 
		{
			for(auto it = quantities->cbegin(); it != quantities->cend(); ++it)
				os<<it->second<<std::endl;
			return Base::Display(os);		
			//~ return Base::Display(Type::Display(os));		
		}
		
		template<typename ValueType>
		const std::string Get(ValueType v,ParaType p)
		{
			  if(*p==Head::UnitType::Sign())
					return Head::UnitType::Name;

              return Base::Instance().Get(v,p);
		}
		
		template<typename Q>
		size_t Size()
		{
			  if(std::is_same<Head,Q>::value)
					return quantities->size();

              return Base::Instance().template Size<Q>();
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
		inline static std::shared_ptr<QuantityMap<Head>> quantities = std::make_shared<QuantityMap<Head>>();
	};

	template<typename Head, typename... Tail>
	std::ostream& operator<<(std::ostream& strm, const QuantityContainer<Head,Tail...> c)
	{
		return c.Display(strm);
	}
	
	//~ template<typename T1, typename T2>
	//~ struct QuantityGet
	//~ {
		//~ template<typename Head, typename... Tail>
		//~ static auto apply()
		//~ {	
			//~ return QuantityGet<T1,typename QuantityContainer<Tail...>::Type>::apply(t.GetTail());
		//~ }
	//~ };
	
	//~ template<typename T>
	//~ struct QuantityGet<T,T>
	//~ {
		//~ template<typename Head, typename... Tail>
		//~ static auto apply(QuantityContainer<Head,Tail...> const& t)
		//~ {
			//~ return t.GetHead();
		//~ }
	//~ };
	
	//~ template<typename T, typename... Types>
	//~ auto Get()
	//~ {
		//~ return QuantityGet<T,typename QuantityContainer<Types...>::Type>::apply();
	//~ };
}
#endif
