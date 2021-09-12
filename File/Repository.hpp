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
			typeContainer.CopyTo(dest);
		}
		
	private:
		static inline TypeContainer typeContainer = TypeContainer();
		
		class TreeParserVisitor: 
			public BaseVisitor,
			public Visitor<FS::DirectoryInfo>,
			public Visitor<FS::FileInfo>
		{
		public:
			virtual void Visit(FS::DirectoryInfo& di) 
			{ 
				FS::Directory::Add(&di);
				Backup::Repository::Map(di.Nodes().cbegin(), di.Nodes().cend()); 
			};
			virtual void Visit(FS::FileInfo& fi) { typeContainer.Add(&fi); };
		};	
		
		static inline TreeParserVisitor treeParser = TreeParserVisitor();
	};
}

#endif
