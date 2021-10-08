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

#ifndef NODE_HPP
#define NODE_HPP

namespace fs = std::filesystem;

namespace FS
{

	//---------------------------------------------------------------------------------------------------Node----------------------------------------------------------------------------------------
	template<typename Derived, typename DerivedInfo,typename T = Derived>
	struct Node
	{
		using Type = Derived;
		using ElementType = T;
		using Cont = std::vector<ElementType>;
	protected:
		static inline Cont cont = Cont();
		const DerivedInfo info;
	public:		
		Node(DerivedInfo* fi): info(*fi){};
		
		static void Add(Info* fi)
		{ 
			cont.push_back(ElementType(static_cast<DerivedInfo*>(fi))); 
		};
		static ElementType Get(Info* fi){return ElementType();};
		static const Cont& Nodes() { return cont; };
		
		const DerivedInfo& Info() const {return info;}
		bool BelongsTo(const fs::path& p) const
		{  
			auto pIt = fs::path(this->info.Path()).begin();
			
			for(auto sp = p.begin(); sp != p.end(); ++sp, ++pIt)
				if(*sp != *pIt)
					return false;
				
			return true;
		};
	};
	
	//---------------------------------------------------------------------------------------------------Directory----------------------------------------------------------------------------------------

	struct Directory: Node<Directory, DirectoryInfo>
	{	
		Directory(DirectoryInfo * di): Node(di){};
	};
	
	//---------------------------------------------------------------------------------------------------File----------------------------------------------------------------------------------------

// 	template<typename ParseT = std::string>
	struct File: Node<File, FileInfo>
	{
		
		File(FileInfo* fi): Node(fi){};

		void CopyTo(std::string destinationName) const 
		{ 
			auto srcName = fs::path(this->info.Path()).parent_path().string() +"/"+ this->info.Name();
			fs::copy(srcName, fs::path(destinationName));
		};
	
		
		std::vector<std::string> Read() const
		{
			std::string line;
			std::vector<std::string> result = std::vector<std::string>();
			
			std::ifstream ifs (this->info.Path());
			if (ifs.is_open())
			{
				while ( getline (ifs,line) )
				{
					result.push_back(line);
				}
				ifs.close();
			}
			
			return result;
		};
		
		template<typename ParseType, typename ParseTypeContainer = ParseType::ParseCont>
		ParseTypeContainer Parse() const
		{
			auto content = this->Read();
			
			return ParseType::Parse(content);
		}
	};

	//---------------------------------------------------------------------------------------------------FileTypes----------------------------------------------------------------------------------------

	template<typename T>
	struct FileTypeBase: Node<FileTypeBase<T>, FileInfo, File>
	{
		using ParseType = std::string;
		using ParseCont = std::vector<ParseType>;
		static const char* Extension;		
	};
	
	struct CPP: public FileTypeBase<CPP>{};
	struct HPP: public FileTypeBase<HPP>{};
	struct H: public FileTypeBase<H>{};
	struct CSV: public FileTypeBase<CSV>{};

	template<> const char* FileTypeBase<CPP>::Extension = ".cpp";
	template<> const char* FileTypeBase<HPP>::Extension = ".hpp";
	template<> const char* FileTypeBase<H>::Extension = ".h";
	template<> const char* FileTypeBase<CSV>::Extension = ".csv";
}


#endif
