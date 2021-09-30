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
#include "FileTypeContainer.hpp"
#include "FileSystem.hpp"
#include "Info.hpp"
#include "Node.hpp"
#include "../Typelist/Typelist.h"
#include "../Visitor/Visitor.hpp"


#ifndef REPOSITORY_HPP
#define REPOSITORY_HPP

namespace Backup
{
	
	struct Repository
	{
		using FileTypes = Typelist<FS::HPP,FS::H,FS::CSV,FS::CPP>::Type;
		using TypeContainer = FS::FileTypeContainer<FileTypes>;
	
		inline static std::string Root = "//home//markus//Dokumente//cpp//Matrix"; 
		inline static std::string Dest = "//home//markus//Downloads"; 
		
		template<typename Iterator>
		static void Map(const Iterator& begin, const Iterator& end)
		{
			for(Iterator it = begin; it != end; ++it)
			{
				(*it)->Accept(treeParser);
			}
			
		}
		
		static void CopyTo(std::string dest)
		{
			typeContainer.SetRootPath(Repository::Root);
			typeContainer.CopyTo(dest);
		}
		
		static void List()
		{
			typeContainer.List();
		}
		
		static void Backup(std::string from, std::string to)
		{
			auto nodes = FileSystem::List(from);

			auto root = fs::directory_entry(from);
			auto dir = new FS::DirectoryInfo(root.path(),root.last_write_time(),nodes);
			
			nodes.push_back(dir);
			Backup::Repository::Map(nodes.cbegin(), nodes.cend());
			
			FileSystem::CreateDirectories(from,to);
			Backup::Repository::List();
			Backup::Repository::CopyTo(to);

			FileSystem::List(to);
		}
		
	private:
		static inline TypeContainer typeContainer = TypeContainer();
				
		class TreeParserVisitor: 
			public BaseVisitor,
			public Visitor<FS::DirectoryInfo>,
			public Visitor<FS::FileInfo>
		{
		public:
			virtual void Visit(FS::DirectoryInfo& di) {	FS::Directory::Add(&di); };
			virtual void Visit(FS::FileInfo& fi) { typeContainer.Add(&fi); };
		};	
		
		static inline TreeParserVisitor treeParser = TreeParserVisitor();
	};
}

#endif
