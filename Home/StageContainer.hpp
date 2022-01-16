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
		inline static const std::string Name = "Stages";
	protected:
		inline static std::unique_ptr<StagesMap, DebugDeleter<StagesMap>> stages = std::unique_ptr<StagesMap, DebugDeleter<StagesMap>>(new StagesMap(),DebugDeleter<StagesMap>());
		
		inline static std::unique_ptr<FS::FileInfo> fileInfo = std::unique_ptr<FS::FileInfo>(new FS::FileInfo(std::filesystem::path(DestinationPath + Name)));
		inline static std::unique_ptr<FS::CSV> csv = std::unique_ptr<FS::CSV>(new FS::CSV(fileInfo.get()));
		StageContainer() 
		{ 
			Read();
			Logger::Log<Info>()<<"StageContainer created."<<Head::Name<<std::endl; 
			Head::Instance(stages->at(Head::Name)); 
			
		};
	public:
		static std::ostream& Display(std::ostream& os) 
		{
			return Type::Instance(stages->at(Head::Name)).Display(os);
		}
		
		void Write(const std::string sourcePath = ".")
		{
			Type::Write(sourcePath);
		}
		
		template<unsigned N>
		auto Get() { return At<StageTypes,N>::Type; }

		static StageContainer& Instance()
		{
			static StageContainer instance;
			return instance;
		}
	private:
		void ExtractValues(const std::vector<std::vector<std::string>>& csvValues)
		{
			auto keysIt = csvValues.at(0).cbegin();
			for(int i = 1; i < csvValues.size(); ++i)
			{
				auto stage = StageMap();
				for(int j = 1; j < csvValues.at(i).size(); ++j)
					stage.insert(std::pair<std::string,std::string>(*(keysIt+j), csvValues.at(i).at(j)));
				
				for(auto kv : stage)
					Logger::Log()<<kv.first<<": "<<kv.second<<std::endl;
				
				stages->insert(std::pair<std::string,StageMap>(csvValues.at(i).at(0), stage));
			}
			
			for(auto kv : *stages)
				Logger::Log()<<kv.first<<std::endl;
			Logger::Log()<<"END: Extract "<<std::endl;
		}
		
		void Read(const std::string sourcePath = ".")
		{
			auto values = csv->Read();
			ExtractValues(values);
		}
		
	};

	template<typename Head, typename... Tail>
	class StageContainer<Typelist<Head,Tail...>>: public StageContainer<Typelist<Tail...>>
	{
	public:
		using Type = Head;
		using StageTypes = Typelist<Head,Tail...>;
		using ContainerType = StageContainer<Typelist<Head,Tail...>>;
		using Base = StageContainer<Typelist<Tail...>>;
	protected:
		StageContainer() 
		{ 
			Logger::Log<Info>()<<"StageContainer created."<<Head::Name<<std::endl; 
			Type::Instance(Base::stages->at(Head::Name)); 
			
		};
	public:
		static std::ostream& Display(std::ostream& os) 
		{
			return StageContainer<Typelist<Tail...>>::Display(Type::Instance(Base::stages->at(Head::Name)).Display(os));		
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