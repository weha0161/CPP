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
#include <boost/filesystem.hpp>
#include <filesystem>
#include "../Logger/Logger.hpp"


#ifndef DIRECTORY_HPP
#define DIRECTORY_HPP

namespace fs = std::filesystem;

namespace FS
{
	std::string to_string(std::filesystem::file_time_type const& ftime) 
	{
// 		std::time_t cftime = std::chrono::system_clock::to_time_t(
// 			std::chrono::file_clock::to_sys(ftime));
// 		std::string str = std::asctime(std::localtime(&cftime));
// 		str.pop_back();  // rm the trailing '\n' put by `asctime`
// 		return str;
		
		return"";
	}
	
	class Node
	{
	protected:
		const std::string name;
		const std::string path;
		const std::filesystem::file_time_type lastModification;
		std::uintmax_t size;
		Node(std::string p, std::uintmax_t s, std::filesystem::file_time_type lm):path(p), size(s), lastModification(lm){  };
		virtual Node* GetChild(int n) { return 0; }
	public:
		virtual long Size() const { return size; }
		const std::string& Name() const{ return name; };
		const std::string& Path() const { return path; };
		const std::string LastModification()const { return to_string(this->lastModification); };
		const std::string virtual Info() const { return this->Path() + std::string("\t") + std::to_string(this->Size()) + std::string("\t") + this->LastModification() ; };
	};
	
	std::ostream& operator<<(std::ostream& out, const Node* n)
	{
		return out<<n->Info();
	}


	class File: public Node
	{
	private:
		const std::string extension;
		const fs::file_time_type lastModification;
	public:
		File(std::string p, std::uintmax_t s, std::filesystem::file_time_type lm):Node(p,s,lm)
		{
		};
	};

	class Directory: public Node
	{   
	private:
		std::vector<Node*> nodes;
	public: 
		Directory(std::string p, std::uintmax_t s, std::filesystem::file_time_type lm):Node(p,s,lm)
		{
			this->size = this->Size();
		};
		
		long Size()
		{
			long result = 0;
			Node* child;
			for(int i = 0; child = GetChild(i); ++i)
				result += child->Size();
				
			return result;
		}
	//         void ReadLines(std::string file);        
		void Map(const fs::path& pathToScan);
	//     static std::vector<std::string> GetAllFiles(std::string pattern, std::string dir = ".");
		
	//         std::vector<std::string> GetAllLines() const;
	//         std::vector<std::string> GetAllFiles(std::string pattern, std::string dir = ".") const;
	};
	
}


// void Directory::Map(const fs::path& pathToScan) {
//     for (const auto& entry : fs::directory_iterator(pathToScan)) {
//         const auto filenameStr = entry.path().filename().string();
//         if (entry.is_directory()) {
//             Directory dir = Directory(entry.path());
//             dir.Map(entry.path());
//             this->directories.push_back(dir);
//         }
//         else
//         {
//             auto file = File(entry.path());
//             this->files.push_back(File(entry.path()));
//             Logger::Instance().Log("Create: "+ file.Name());
//         }
//     }
// }

// std::vector<std::string> Directory::GetAllFiles(std::string pattern, std::string dir)
// {
//     Logger::Instance()<<"GetAllFiles with pattern: " + pattern + " in directory: " + dir;
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
//             Logger::Instance()<<"Error in FileService GetAllFiles.";
//             exit (EXIT_FAILURE);
//     }
//     Logger::Instance()<<"GetAllFiles found: "+std::to_string(result.size());
// 
//     if (num_entries) {
//         std::cout<<"GetAllFiles found: "+std::to_string(result.size());
//         for(i=0, list=namelist; i < num_entries; i++, list++) 
//         {
//             result.push_back( (*list)->d_name);
//             Logger::Instance()<<"GetAllFiles found file: "+std::string((*list)->d_name);
//             std::cout<<"GetAllFiles found file: "+std::string((*list)->d_name)<<std::endl;
//             free (*list);
//         }
//         free (namelist);
//     }
//     
//     return result; 
// }


#endif
