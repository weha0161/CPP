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
#include "Info.hpp"
#include "../Logger/Logger.hpp"
#include "../Typelist/Typelist.h"
#include "../Visitor/Visitor.hpp"

#ifndef NODE_HPP
#define NODE_HPP

namespace fs = std::filesystem;

namespace FS
{

	static std::vector<std::string> ReadLines(std::string path)
	{
		std::string line;
		auto result = std::vector<std::string>();			
		
		std::ifstream ifs (path);
		if (ifs.is_open())
		{
			while ( getline (ifs,line) )
			{
				result.push_back(line);
			}
			ifs.close();
		}
		
		return result;
	}
	
	static void WriteLines(std::string path, std::vector<std::string> lines)
	{
		std::ofstream ofs(path);
		if (ofs.is_open())
		{
			for(auto line : lines)
				ofs<<line<<std::endl;
			
			ofs.close();
		}
	}
	
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
			{
				if(*sp != *pIt)
					return false;
			}
				
			return true;
		};
	};
	
	//---------------------------------------------------------------------------------------------------Directory----------------------------------------------------------------------------------------

	struct Directory: Node<Directory, DirectoryInfo>
	{	
		Directory(DirectoryInfo * di): Node(di){};
	};
	
	//---------------------------------------------------------------------------------------------------File----------------------------------------------------------------------------------------

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
			return FS::ReadLines(this->info.Path());
		};
		
		void Write(std::vector<std::string> lines)
		{
			
		};
		
		template<typename ParseType, typename ParseTypeContainer = ParseType::ParseCont>
		ParseTypeContainer Parse() const
		{
			auto content = this->Read();
			
			ParseType::Parse(content);
			
			return ParseType::Transactions;
		}
	};

	//---------------------------------------------------------------------------------------------------FileTypes----------------------------------------------------------------------------------------

	template<typename FileT>
	struct FileTypeBase: Node<FileTypeBase<FileT>, FileInfo, File>
	{
		FileTypeBase(FileInfo* fi): Node<FileTypeBase<FileT>, FileInfo, File>(fi){};
		using ParseType = std::string;
		using ParseCont = std::vector<ParseType>;
		static const char* Extension;		
		
		template<typename Separator = T::char_<';'>>
		static std::vector<std::string> ExtractValues(std::string line)
		{
			return String_::Split<Separator>(line);
		};
	};
	
	struct CTRV: public FileTypeBase<CTRV>{};
	struct KEYS: public FileTypeBase<KEYS>{};
	struct CPP: public FileTypeBase<CPP>{};
	struct HPP: public FileTypeBase<HPP>{};
	struct H: public FileTypeBase<H>{};
	
	struct CSV: public FileTypeBase<CSV>
	{
		CSV(FileInfo* fi): FileTypeBase(fi), destinationPath(this->Info().Path() + CSV::Extension){};

		template<typename Ctr, typename Separator = T::char_<';'>>
		void Write()
// 		void Write(const Ctr& counter)
		{
			std::unique_ptr<std::ofstream> ofs = std::unique_ptr<std::ofstream>(new std::ofstream(destinationPath)); 
			
			Ctr::template Display<Separator>(*ofs);
			
			ofs->close();
		}
		
		const std::string& GetDestinationPath(){ return this->destinationPath; };
	private:
		const std::string destinationPath;
	};

	
	template<> const char* FileTypeBase<CTRV>::Extension = ".ctrv";
	template<> const char* FileTypeBase<KEYS>::Extension = ".keys";
	template<> const char* FileTypeBase<CPP>::Extension = ".cpp";
	template<> const char* FileTypeBase<HPP>::Extension = ".hpp";
	template<> const char* FileTypeBase<H>::Extension = ".h";
	template<> const char* FileTypeBase<CSV>::Extension = ".csv";
}


#endif
