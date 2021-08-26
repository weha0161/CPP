#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <unordered_map>
#include <boost/filesystem.hpp>
#include <filesystem>
#include "Logger.h"


#ifndef DIRECTORY_HPP
#define DIRECTORY_HPP

namespace fs = std::filesystem;

namespace FS
{
	class Node
	{
	protected:
		const std::string name;
		const std::string path;
		Node(){  };
		virtual Node* GetChild(int n) { return 0; }
	public:
		virtual long Size() { return 0; }
		const std::string& Name(){ return name; };
		const std::string& Path(){ return path; };
	};

	class File: public Node
	{
	private:
		const std::string extension;
		const fs::file_time_type lastModification;
	public:
		File(){};
	};

	class Directory: Node
	{   
	private:
		std::vector<Node*> nodes;
	public: 
		Directory(){};
		
		Directory(std::string fp){};
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
