	#include <algorithm>
	#include <functional>
	#include <iostream>
	#include <fstream>
	#include <chrono>
	#include <ctime>
	#include <iterator>
	#include <vector>
	#include <cstdlib>
	#include <filesystem>
	#include "Stage.hpp"
	#include "../Logger/Logger.hpp"
	#include "../Typelist/Typelist.h"

	#ifndef STAGECONTAINER_HPP
	#define STAGECONTAINER_HPP

	//---------------------------------------------------------------------------------------------------StageContainer----------------------------------------------------------------------------------------

	using StagesMap = std::map<std::string, StageMap>;

	template<typename List>
	class StageContainer{};

	template<typename Head>
	class StageContainer<Typelist<Head>>
	{
	public:
		using Type = Head;
		using StageTypes = Typelist<Head>;
		using ContainerType = StageContainer<Typelist<Head>>;
	private:
		inline static const std::string DestinationPath = "//home//markus//Downloads//";
		inline static const std::string Name = "18061860_Water";
	protected:
		inline static std::unique_ptr<StagesMap, DebugDeleter<StagesMap>> stages = std::unique_ptr<StagesMap, DebugDeleter<StagesMap>>(new StagesMap(),DebugDeleter<StagesMap>());
		
		inline static std::unique_ptr<FS::FileInfo> fileInfo = std::unique_ptr<FS::FileInfo>(new FS::FileInfo(std::filesystem::path(DestinationPath + Name)));
		inline static std::unique_ptr<FS::CSV> csv = std::unique_ptr<FS::CSV>(new FS::CSV(fileInfo.get()));
		StageContainer() 
		{ 
			Read();
			stages.get();
			Head::Instance(StageMap()); 
			Logger::Log<Info>()<<"StageContainer created."<<std::endl; 
			
		};
	public:
		static std::ostream& Display(std::ostream& os) 
		{
			return Type::Display(os);
		}
		
		void Write(const std::string sourcePath = ".")
		{
			Type::Write(sourcePath);
		}
		
		void Read(const std::string sourcePath = ".")
		{
			auto values = csv->Read();
			Logger::Log()<<values.size()<<std::endl;
		}
		
		template<unsigned N>
		auto Get() { return At<StageTypes,N>::Type; }

		static StageContainer& Instance()
		{
			static StageContainer instance;
			return instance;
		}		
	};

	template<typename Head, typename... Tail>
	class StageContainer<Typelist<Head,Tail...>>: public StageContainer<Typelist<Tail...>>
	{
	public:
		using Type = Head;
		using StageTypes = Typelist<Head,Tail...>;
		using ContainerType = StageContainer<Typelist<Head,Tail...>>;
	protected:
		StageContainer() 
		{ 
			StageContainer<Typelist<Tail...>>::stages.get();
			Head::Instance(StageMap()); 
			Logger::Log<Info>()<<"StageContainer created."<<std::endl; 
			
		};
	public:
		static std::ostream& Display(std::ostream& os) 
		{
			return StageContainer<Typelist<Tail...>>::Display(Type::Display(os));		
		}
		
		template<unsigned N>
		auto Get() { return At<StageTypes,N>::Type; }
	
		static StageContainer& Instance()
		{
			static StageContainer instance;
			return instance;
		}	
	};

	template<typename Head, typename... Tail>
	std::ostream& operator<<(std::ostream& strm, const StageContainer<Head,Tail...> c)
	{
		return c.Display(strm);
	}

	using StageContainerType = StageContainer<Typelist<Top,Middle,Bottom>>::ContainerType;

	#endif
