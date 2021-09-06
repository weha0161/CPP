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
#include "Directory.hpp"
#include "../Typelist/Typelist.h"
#include "../Visitor/Visitor.hpp"


#ifndef REPOSITORY_HPP
#define REPOSITORY_HPP

namespace fs = std::filesystem;
// namespace mpl = boost::mpl;
// using namespace mpl::placeholders;

namespace Backup
{
	
	struct Repository
	{
		using FileTypes = Typelist<FS::HPP,FS::H,FS::CSV,FS::CPP>::Type;
		using TypeContainer = FS::FileTypeContainer<FileTypes>;
		
		
		template<typename Iterator>
		static void Map(const Iterator& begin, const Iterator& end)
		{
			for(Iterator it = begin; begin != end; ++it)
			{
				try
				{
					(*it)->Accept(treeParser);
				}
				catch(...)
				{
					Logger::Log()<<"Map end"<<std::endl;
				}
			}
			
		}
		
	private:
		static inline TypeContainer typeContainer = TypeContainer();
		
		class TreeParserVisitor: 
			public BaseVisitor,
			public Visitor<FS::DirectoryInfo>,
			public Visitor<FS::FileInfo>
		{
		public:
			virtual void Visit(FS::DirectoryInfo& di) { Logger::Log<Debug>()<<"DirectoryInfo"<<std::endl; };
			virtual void Visit(FS::FileInfo& fi) { typeContainer.Add(&fi); };
		};	
		
		static inline TreeParserVisitor treeParser = TreeParserVisitor();
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
