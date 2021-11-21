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
#include "Account.hpp"
#include "Comdirect.hpp"
#include "../Typelist/Typelist.h"
#include "../Visitor/Visitor.hpp"


#ifndef REPOSITORY_HPP
#define REPOSITORY_HPP

namespace CSV
{
	
	struct Repository
	{
		using FileTypes = Typelist<FS::CSV>::Type;
		using TypeContainer = FS::FileTypeContainer<FileTypes>;
		using Direction = FS::Transfer<FS::In>;
		using ParseTypes = Typelist<FS::Custom<0>, FS::Raiba<0>, FS::Comdirect<0>>::Type;
		using ParseTypeContainer = FS::FileTypeContainer<ParseTypes>;
		using ParseMethod = void(*)(std::vector<std::string>);
		using ParserContainer = std::map<std::string, ParseMethod>;
	
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
		
		static void SetRootPath(std::string s) { Root = s; }
		static void SetDestPath(std::string s) { Dest = s; }
		
		static void Map(std::string path)
		{
			auto nodes = FileSystem::List(path);

			auto root = fs::directory_entry(path);
			auto dir = new FS::DirectoryInfo(root.path(),root.last_write_time(),nodes);
			
			nodes.push_back(dir);
			CSV::Repository::Map(nodes.cbegin(), nodes.cend());
		}
		
		static void Backup(std::string from, std::string to)
		{
			Repository::Root = from;
			Repository::Dest = to;
			
			Repository::Map(from);
			
			FileSystem::CreateDirectories(from,to);
			CSV::Repository::List();
			CSV::Repository::CopyTo(to);
		}
		
		static std::vector<std::string> Read(std::string s)
		{
			return typeContainer.Read(s);			
		}
		
		static void Attach()
		{
			parseTypeContainer.AttachTo(Repository::parseContainer);	
		}
		
		template<typename ParseType>
		static typename ParseType::ParseCont Parse(std::string s)
		{
			return typeContainer.Parse<ParseType>(s);			
		}
		
		static void ParseAll()
		{
			for (auto it = parseContainer.begin(); it != parseContainer.end(); it++)
			{
				auto lines = CSV::Repository::Read(it->first);				
				it->second(lines);
			}			
		}
		
		static void Display(std::ostream& os)
		{
			parseTypeContainer.Display(os);
		}
		
	private:
		static inline ParserContainer parseContainer = ParserContainer();
		static inline TypeContainer typeContainer = TypeContainer();
		static inline ParseTypeContainer parseTypeContainer = ParseTypeContainer();
		inline static std::string Root = ""; 
		inline static std::string Dest = ""; 
				
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
			typeContainer.SetRootPath(Repository::Root);
			typeContainer.CopyTo(dest);
		}
		
		static void List()
		{
			typeContainer.List();
		}
		
		static void SetRootPath(std::string s) { Root = s; }
		static void SetDestPath(std::string s) { Dest = s; }
		
		static void Backup(std::string from, std::string to)
		{
			Repository::Root = from;
			Repository::Dest = to;
			
			auto nodes = FileSystem::List(from);

			auto root = fs::directory_entry(from);
			auto dir = new FS::DirectoryInfo(root.path(),root.last_write_time(),nodes);
			
			nodes.push_back(dir);
			Backup::Repository::Map(nodes.cbegin(), nodes.cend());
			
			FileSystem::CreateDirectories(from,to);
			Backup::Repository::List();
			Backup::Repository::CopyTo(to);

// 			FileSystem::List(to);
		}
		
		static std::vector<std::string> Read(std::string s)
		{
			return typeContainer.Read(s);			
		}
		
		template<typename ParseType>
		static typename ParseType::ParseCont Parse(std::string s)
		{
			return typeContainer.Parse<ParseType>(s);			
		}
		
	private:
		static inline TypeContainer typeContainer = TypeContainer();
		inline static std::string Root = ""; 
		inline static std::string Dest = ""; 
				
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
