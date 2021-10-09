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
// #include "Visitor.h"

#ifndef DIRECTORY_H
#define DIRECTORY_H

namespace fs = std::filesystem;
namespace mpl = boost::mpl;
using namespace mpl::placeholders;

namespace FS
{
	class BaseVisitor
	{
	public:
		virtual ~BaseVisitor(){}
	};
	
	template<class T, typename R = void>
	class Visitor
	{
	public:
		using ReturnType = R;
		virtual ReturnType Visit(T&) = 0;
	};
	
	template<typename R = void>
	class BaseVisitable
	{
	public:
		using ReturnType = R;
		virtual ~BaseVisitable(){}
		virtual ReturnType Accept(BaseVisitor&) = 0;
	protected:
		template<class T>
		static ReturnType AcceptImpl(T& visited, BaseVisitor& visitor)
		{
			if(Visitor<T,R>* p = dynamic_cast<Visitor<T,R>*>(&visitor))
				return p->Visit(visited);
			
			return ReturnType();
		}		
	};
	
	template <typename TP>
	std::time_t to_time_t(TP tp)
	{
		using namespace std::chrono;
		auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
				+ system_clock::now());
		return system_clock::to_time_t(sctp);
	};
	
	std::string to_timestring(std::time_t tt)
	{
		std::tm *gmt = std::gmtime(&tt);
		std::stringstream buffer;
		buffer << std::put_time(gmt, "%A, %d %B %Y %H:%M");
		std::string formattedFileInfoTime = buffer.str();
		return formattedFileInfoTime;
	};
	
//---------------------------------------------------------------------------------------------------Info----------------------------------------------------------------------------------------
	class Info : public BaseVisitable
	{
	protected:
		const std::string name;
		const std::string path;
		std::filesystem::path fs_path;
		const std::filesystem::file_time_type lastModification;
		std::uintmax_t size;
		Info(std::filesystem::path p, std::uintmax_t s, std::filesystem::file_time_type lm);
		virtual Info* Child(int n) { return 0; }
	public:
		virtual long Size() const {return size; };
		virtual ReturnType Accept(BaseVisitor& visitor) { return AcceptImpl(*this, visitor); }
// 		DEFINE_VISITABLE();
		Info();
		virtual ~Info();
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
		const std::string extension;
		const fs::file_time_type lastModification;
	public:
// 		DEFINE_VISITABLE();
		virtual ReturnType Accept(BaseVisitor& visitor) { return AcceptImpl(*this, visitor); }
		
		FileInfo();
		~FileInfo();
		FileInfo(std::filesystem::path p, std::filesystem::file_time_type lm, std::uintmax_t s);
	};
//---------------------------------------------------------------------------------------------------DirectoryInfo----------------------------------------------------------------------------------------

	class DirectoryInfo : public Info
	{   
	private:
		std::vector<Info*> nodes;
	public: 
// 		DEFINE_VISITABLE();
		virtual ReturnType Accept(BaseVisitor& visitor) { return AcceptImpl(*this, visitor); }
		
		~DirectoryInfo();
		DirectoryInfo(std::filesystem::path p, std::filesystem::file_time_type lm, std::vector<Info*> n);
		
		long Size()
		{
			long result = 0;
			//Info* child;
			//for(auto it = nodes.cbegin(); it != nodes.cend(); ++it)
				//result += (*it)->Size();
			
// 			Logger::Log<Debug>()<<this->nodes.size()<<"\t"<<result<<std::endl;
				
			return result;
		}
		
	//         void ReadLines(std::string file);        
		//void Map(const fs::path& pathToScan){};
	//     static std::vector<std::string> GetAllFileInfos(std::string pattern, std::string dir = ".");
		
	//         std::vector<std::string> GetAllLines() const;
	//         std::vector<std::string> GetAllFileInfos(std::string pattern, std::string dir = ".") const;
	};
	
	
	class TreeParserVisitor: 
		public BaseVisitor,
		public Visitor<DirectoryInfo>,
		public Visitor<FileInfo>
	{
		virtual void visit(DirectoryInfo& di) { Logger::Log<Debug>()<<"DirectoryInfo"<<std::endl; };
		virtual void visit(FileInfo& fi) { Logger::Log<Debug>()<<"FileInfo"<<std::endl; };
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
		
		void Add(FileInfo fi){};
		FileType Get(FileInfo fi){return FileType();};
	private:
		Cont cont;
	};
	
	struct CPP: public FileTypeBase<CPP>{};
	struct HPP: public FileTypeBase<CPP>{};
	struct H: public FileTypeBase<CPP>{};
	struct CSV: public FileTypeBase<CPP>{};

	
	//---------------------------------------------------------------------------------------------------Directory----------------------------------------------------------------------------------------

	struct Visit_Type
	{
		FileInfo fileInfo;
		
		Visit_Type(FileInfo fi): fileInfo(fi){};
		
		template<class Visitor>
		void operator()(Visitor)
		{
			Visitor::visit();
		};
	};
	
	template<class T>
	struct Add_Visitor
	{
		static void visit()
		{
			std::cout<<"Test"<<std::endl;
		};
	};
	
	struct Directory
	{
// 		using types = mpl::vector<CPP,HPP>;
		using types = mpl::vector<std::vector<Directory>, std::vector<File<CPP>>, std::vector<File<HPP>>>;
		
		void Add()
		{
// 			auto f = FileInfo();
// 			auto a = Visit_Type(f);
// 			mpl::for_each<types, Add_Visitor<_1>>(a);
			
		}
	};
}

// void DirectoryInfo::Map(const fs::path& pathToScan) {
//     for (const auto& entry : fs::directory_iterator(pathToScan)) {
//         const auto filenameStr = entry.path().filename().string();
//         if (entry.is_directory()) {
//             DirectoryInfo dir = DirectoryInfo(entry.path());
//             dir.Map(entry.path());
//             this->directories.push_back(dir);
//         }
//         else
//         {
//             auto file = FileInfo(entry.path());
//             this->files.push_back(FileInfo(entry.path()));
//             Logger::Instance().Log("Create: "+ file.Name());
//         }
//     }
// }

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
