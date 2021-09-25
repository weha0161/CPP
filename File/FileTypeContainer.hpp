#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <unordered_map>
#include <filesystem>
#include "../Logger/Logger.hpp"
#include "../Typelist/Typelist.h"
#include "../Visitor/Visitor.hpp"
#include "Info.hpp"

#ifndef FILETYPECONTAINER_HPP
#define FILETYPECONTAINER_HPP

namespace FS
{
	//---------------------------------------------------------------------------------------------------FileTypeContainer----------------------------------------------------------------------------------------
	template<typename List>
	class FileTypeContainer	{};
	
	template<typename Head>
	class FileTypeContainer<Typelist<Head>>
	{
	protected:
		std::string rootPath;
		std::filesystem::path BuildDestPath(const std::string& src, const std::string &dst)
		{
			auto srcPath = std::filesystem::path(src);
			auto rootP = std::filesystem::path(rootPath);
			auto dstPath = std::filesystem::path(dst);
			std::string result = dst;

			auto it2 = rootP.begin();

			for (auto it = srcPath.begin() ; it != srcPath.end(); ++it, ++it2)
			{
				if((*(--rootP.end())) == *it || *it != *it2)
					result = result + "/" + (*it).string();
			}
				
			return std::filesystem::path(result);
		}
	public:
		void SetRootPath(std::string p){ this->rootPath = p;}
		using Type = Head;
		
		void Add(FileInfo* fi)
		{
			if(strcmp(Type::Extension, fi->Extension()) == 0)
				Head::Add(fi); 
		}
		
		void CopyTo(std::string dest)
		{
			for(auto it = Head::Nodes().cbegin(); it != Head::Nodes().cend(); ++it)
			{
				std::string dst = this->BuildDestPath(it->Info().Path(),dest);
				it->CopyTo(dest);
			}
		}
		
		void List()
		{
			for(auto it = Head::Nodes().cbegin(); it != Head::Nodes().cend(); ++it)
			{				
				Logger::Log()<<it->Info()<<std::endl;
			}			
		}
		
		FileTypeContainer()	{ }
	};
	
	template<typename Head, typename... Tail>
	class FileTypeContainer<Typelist<Head,Tail...>>: public FileTypeContainer<Typelist<Tail...>>
	{
	public:
		using Type = Head;

		void Add(FileInfo* fi)
		{
			if(strcmp(Type::Extension, fi->Extension()) == 0)
			{
				Head::Add(fi); 
			}
			else
			{
				FileTypeContainer<Typelist<Tail...>>::Add(fi);
			}				
		}
		
		void CopyTo(std::string dest)
		{
			for(auto it = Head::Nodes().cbegin(); it != Head::Nodes().cend(); ++it)
			{
				std::string dst = this->BuildDestPath(it->Info().Path(),dest);
// 				Logger::Log()<<"Destination in CopyTO: "<<dst<<std::endl;
				it->CopyTo(dst);
			}
			
			FileTypeContainer<Typelist<Tail...>>::CopyTo(dest);
		}
		
		void List()
		{
			for(auto it = Head::Nodes().cbegin(); it != Head::Nodes().cend(); ++it)
			{				
				Logger::Log()<<it->Info()<<std::endl;
			}
			
			FileTypeContainer<Typelist<Tail...>>::List();
		}
		
		FileTypeContainer() { };
	};

}


#endif
