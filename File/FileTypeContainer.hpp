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
		std::string BuildDestPath(const std::string& src, const std::string &dst)
		{
			std::string result = src;
			return result.replace(0, this->rootPath.size(), dst);
		}
	public:
		void SetRootPath(std::string p){ this->rootPath = p;}
		using Type = Head;
		
		void Add(FileInfo* fi)
		{
			if(strcmp(Type::Extension, fi->Extension()) == 0)
			{
				Head::Add(fi); 
// 				Logger::Log<Debug>()<<Head::Extension<<" "<<fi<<std::endl;
			}
		}
		
		void CopyTo(std::string dest)
		{
			for(auto it = Head::Nodes().cbegin(); it != Head::Nodes().cend(); ++it)
				it->CopyTo(dest);
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
				Logger::Log<Debug>()<<"CopyTo"<<this->rootPath<<" "<<it->Info().Path()<<" "<<dst<<std::endl;
				it->CopyTo(dst);
			}
			
			FileTypeContainer<Typelist<Tail...>>::CopyTo(dest);
		}
		
		FileTypeContainer() { };
	};

}


#endif
