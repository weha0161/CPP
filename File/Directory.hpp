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

		virtual Info* Child(int n) = 0;
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
		
	protected:
		virtual Info* Child(int n) { return 0; }
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
	protected:
		virtual Info* Child(int n) { return 0; }
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
		
		const std::vector<Info*>& Nodes() { return this->nodes; }
	};
	
	//---------------------------------------------------------------------------------------------------Node----------------------------------------------------------------------------------------
	template<typename Derived, typename T = Derived>
	struct Node
	{
		using Type = Derived;
		using ELementType = T;
		using Cont = std::vector<ELementType>;
		
		static void Add(Info* fi){ };
		ELementType Get(Info* fi){return ELementType();};
	protected:
		Cont cont;
	};
	
	//---------------------------------------------------------------------------------------------------Directory----------------------------------------------------------------------------------------

	struct Directory: Node<Directory>
	{
	};
	
	//---------------------------------------------------------------------------------------------------File----------------------------------------------------------------------------------------

	struct File
	{
		const FileInfo fileInfo;
		File(const FileInfo& fi): fileInfo(fi){};
		const FileInfo& Info() const {return fileInfo;}
	};

	//---------------------------------------------------------------------------------------------------FileTypes----------------------------------------------------------------------------------------

	template<typename T>
	struct FileTypeBase: Node<FileTypeBase<T>, File>
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


#endif
