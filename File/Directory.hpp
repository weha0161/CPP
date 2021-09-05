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

#ifndef DIRECTORY_H
#define DIRECTORY_H

namespace fs = std::filesystem;
namespace mpl = boost::mpl;
using namespace mpl::placeholders;

namespace FS
{
//---------------------------------------------------------------------------------------------------time_Conversion----------------------------------------------------------------------------------------
	template <typename TP>
	std::time_t to_time_t(TP tp)
	{
		using namespace std::chrono;
		auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
				+ system_clock::now());
		return system_clock::to_time_t(sctp);
	}
	
	std::string to_timestring(std::time_t tt)
	{
		std::tm *gmt = std::gmtime(&tt);
		std::stringstream buffer;
		buffer << std::put_time(gmt, "%A, %d %B %Y %H:%M");
		std::string formattedFileInfoTime = buffer.str();
		return formattedFileInfoTime;
	}
	
//---------------------------------------------------------------------------------------------------Info----------------------------------------------------------------------------------------
	class Info : public BaseVisitable<>
	{
	protected:
		const std::string name;
		const std::string path;
		std::filesystem::path fs_path;
		const std::filesystem::file_time_type lastModification;
		std::uintmax_t size;

		Info(std::filesystem::path p, std::filesystem::file_time_type lm, std::uintmax_t s): fs_path(p), name(p.filename()), path(p), size(s), lastModification(lm){ };

		virtual Info* Child(int n) { return 0; }
	public:
		DEFINE_VISITABLE();
		virtual ~Info(){};
		
		virtual long Size() const {return size; };
		const std::string& Name() const{ return name; };
		const std::string& Path() const { return path; };
		const std::time_t LastModification()const { return to_time_t(this->lastModification); };
		const std::string virtual PrintInfo() const { return this->Name() + std::string("\t") + std::to_string(this->Size()) + std::string("\t") + to_timestring(this->LastModification()) ; };
	};
	
	std::ostream& operator<<(std::ostream& out, const Info* n)
	{
		return out<<n->PrintInfo();
	}

//---------------------------------------------------------------------------------------------------FileInfo----------------------------------------------------------------------------------------

	class FileInfo : public Info
	{
	private:
		char* extension;
		const fs::file_time_type lastModification;
	public:
		DEFINE_VISITABLE();
		~FileInfo(){};

		FileInfo(std::filesystem::path p, std::filesystem::file_time_type lm, std::uintmax_t s): Info(p,lm, s)
		{ 
			size_t length = strlen( p.extension().c_str() );
			extension = new char[ length + 1 ];
			strcpy( extension, p.extension().c_str() );
		};
		
		const char*  Extension() const { return this->extension; };
	};
	
	std::ostream& operator<<(std::ostream& out, const FileInfo* n)
	{
		return out<<n->PrintInfo();
	}
	
//---------------------------------------------------------------------------------------------------DirectoryInfo----------------------------------------------------------------------------------------

	class DirectoryInfo : public Info
	{   
	private:
		std::vector<Info*> nodes;
	public: 
		DEFINE_VISITABLE();
		~DirectoryInfo(){};
		
		DirectoryInfo(std::filesystem::path p, std::filesystem::file_time_type lm, std::vector<Info*> n):Info(p,lm, 0), nodes(n)
		{
				this->size = this->Size();
		};
		
		long Size() const
		{
			long result = 0;
			Info* child;
			for(auto it = nodes.cbegin(); it != nodes.cend(); ++it)
				result += (*it)->Size();
							
			return result;
		}
	};
	
	//---------------------------------------------------------------------------------------------------File<T>----------------------------------------------------------------------------------------

	template<typename FileType>
	struct File
	{
		const FileInfo fileInfo;
		File(const FileInfo& fi): fileInfo(fi){};
		File(){};
		const FileInfo& Info() const {return fileInfo;}
	};

	//---------------------------------------------------------------------------------------------------FileTypes----------------------------------------------------------------------------------------

	template<typename T>
	struct FileTypeBase
	{
		using Type = T;
		using FileType = File<Type>;
		using Cont = std::vector<FileType>;
		static const char* Extension;
		
		static void Add(FileInfo* fi){ };
		FileType Get(FileInfo fi){return FileType();};
	private:
		Cont cont;
	};
	
	struct CPP: public FileTypeBase<CPP>{};
	struct HPP: public FileTypeBase<HPP>{};
	struct H: public FileTypeBase<H>{};
	struct CSV: public FileTypeBase<CSV>{};

	template<> const char* FileTypeBase<CPP>::Extension = ".cpp";
	template<> const char* FileTypeBase<HPP>::Extension = ".hpp";
	template<> const char* FileTypeBase<H>::Extension = ".h";
	template<> const char* FileTypeBase<CSV>::Extension = ".csv";
	
	//---------------------------------------------------------------------------------------------------Directory----------------------------------------------------------------------------------------

	struct Directory
	{
	};
	
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
				Logger::Log<Debug>()<<Head::Extension<<" "<<fi<<std::endl;
			}
		}
		
		FileTypeContainer()	{ }
	};
	
	template<typename Head, typename... Tail>
	class FileTypeContainer<Typelist<Head,Tail...>>: FileTypeContainer<Typelist<Tail...>>
	{
	public:
		using Type = Head;

		void Add(FileInfo* fi)
		{
			if(strcmp(Type::Extension, fi->Extension()) == 0)
			{
				Head::Add(fi); 
				Logger::Log<Debug>()<<Head::Extension<<" "<<fi<<std::endl;
			}
			else
			{
				FileTypeContainer<Typelist<Tail...>>::Add(fi);
			}				
		}
		
		FileTypeContainer() { };
	};

}


// std::vector<std::string> DirectoryInfo::GetAllFileInfos(std::string pattern, std::string dir)
// {
//     Logger::Instance()<<"GetAllFileInfos with pattern: " + pattern + " in directory: " + dir;
//     std::vector<std::string> result;
//     int num_entries, i;
//     struct dirent **namelist, **list;
//     const char *ptr = dir.c_str();
//     char wd[MAX];
//     
//     auto selector = [pattern](const struct dirent *dp)
//         {
//             std::filesystem::path fileObj{dp->d_name};
//             if (fileObj.extension().string().compare(pattern) == 0)
//             {
//                 return 1;
//             }
//             else
//                 return 0;
//         };
//         
//     static auto static_variable = selector;
//     int (*staticSelector)( const struct dirent* ) = []( const struct dirent* a ) { return static_variable( a ) ;};
// 
//     if ((num_entries=scandir(ptr, &namelist, staticSelector, alphasort)) < 0) {
//             Logger::Instance()<<"Error in FileInfoService GetAllFileInfos.";
//             exit (EXIT_FAILURE);
//     }
//     Logger::Instance()<<"GetAllFileInfos found: "+std::to_string(result.size());
// 
//     if (num_entries) {
//         std::cout<<"GetAllFileInfos found: "+std::to_string(result.size());
//         for(i=0, list=namelist; i < num_entries; i++, list++) 
//         {
//             result.push_back( (*list)->d_name);
//             Logger::Instance()<<"GetAllFileInfos found file: "+std::string((*list)->d_name);
//             std::cout<<"GetAllFileInfos found file: "+std::string((*list)->d_name)<<std::endl;
//             free (*list);
//         }
//         free (namelist);
//     }
//     
//     return result; 
// }


#endif
