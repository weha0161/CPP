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
#include <boost/mpl/vector.hpp>
#include <boost/filesystem.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/placeholders.hpp>
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
	public:
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
// 				Logger::Log<Debug>()<<Head::Extension<<" "<<fi<<std::endl;
			}
			else
			{
				FileTypeContainer<Typelist<Tail...>>::Add(fi);
			}				
		}
		
		void CopyTo(std::string dest)
		{
			for(auto it = Head::Nodes().cbegin(); it != Head::Nodes().cend(); ++it)
				it->CopyTo(dest);
			
			FileTypeContainer<Typelist<Tail...>>::CopyTo(dest);
		}
		
		FileTypeContainer() { };
	};

}


#endif
