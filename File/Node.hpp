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
	public:
		
		static void Add(Info* fi)
		{ 
			cont.push_back(ElementType(static_cast<DerivedInfo*>(fi))); 
		};
		
		static ElementType Get(Info* fi){return ElementType();};
		static const Cont& Nodes() { return cont; };
	};
	
	//---------------------------------------------------------------------------------------------------Directory----------------------------------------------------------------------------------------

	struct Directory: Node<Directory, DirectoryInfo>
	{
		const DirectoryInfo directoryInfo;
		Directory(DirectoryInfo* fi): directoryInfo(*fi){};
		const DirectoryInfo& Info() const {return directoryInfo;}
		
		bool BelongsTo(const fs::path& p) const
		{  
			auto pIt = fs::path(this->directoryInfo.Path()).begin();
			
			for(auto sp = p.begin(); sp != p.end(); ++sp, ++pIt)
				if(*sp != *pIt)
					return false;
				
			return true;
		};
	};
	
	//---------------------------------------------------------------------------------------------------File----------------------------------------------------------------------------------------

	struct File
	{
		const FileInfo fileInfo;
		File(FileInfo* fi): fileInfo(*fi){};
		const FileInfo& Info() const {return fileInfo;}
		void CopyTo(std::string destinationName) const 
		{ 
			auto srcName = fs::path(this->fileInfo.Path()).parent_path().string() +"/"+ this->fileInfo.Name();
			fs::copy(srcName, fs::path(destinationName));
		};
	
		bool BelongsTo(const fs::path& p) const
		{  
			auto pIt = fs::path(this->fileInfo.Path()).begin();
			
			for(auto sp = p.begin(); sp != p.end(); ++sp, ++pIt)
				if(*sp != *pIt)
					return false;
				
			return true;
		};
	};

	//---------------------------------------------------------------------------------------------------FileTypes----------------------------------------------------------------------------------------

	template<typename T>
	struct FileTypeBase: Node<FileTypeBase<T>, FileInfo, File>
	{
		static const char* Extension;		
	};
	
	struct CPP: public FileTypeBase<CPP>, public File{};
	struct HPP: public FileTypeBase<HPP>, public File{};
	struct H: public FileTypeBase<H>, public File{};
	struct CSV: public FileTypeBase<CSV>, public File{};

	template<> const char* FileTypeBase<CPP>::Extension = ".cpp";
	template<> const char* FileTypeBase<HPP>::Extension = ".hpp";
	template<> const char* FileTypeBase<H>::Extension = ".h";
	template<> const char* FileTypeBase<CSV>::Extension = ".csv";
}


#endif
