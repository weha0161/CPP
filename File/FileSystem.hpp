#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <memory>
#include "../Logger/Logger.hpp"
#include "../String/String_.hpp"
#include "../Wrapper/Wrapper.hpp"

#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

class FileSystem
{
	using Delimiter = T::char_<'/'> ;
	inline static std::vector<FS::Info*> nodes;
	inline static int level = 0;
		
public:

	static std::vector<FS::Info*> List(const fs::path& pathToScan) {
		std::cout<<"|->"<<pathToScan<<std::endl;
		
		for (const auto& entry : fs::directory_iterator(pathToScan)) {
			const auto filenameStr = entry.path().filename().string();
			
			for(int i = 0; i < level; ++i)
				std::cout<<"|";
				
			
			if (entry.is_directory()) {
				++level;
				auto dirnodes = FileSystem::List(entry.path());
				FS::DirectoryInfo* dir = new FS::DirectoryInfo(entry.path(),entry.last_write_time(),dirnodes);
				nodes.push_back(dir);
			}
			else
			{
				FS::FileInfo* file = new FS::FileInfo(entry.path(), entry.last_write_time(), entry.file_size());
				nodes.push_back(file);
				std::cout<<"|--"<<*file<<std::endl;
			}
			
		}
		
		--level;
		
		return nodes;
	}
	
	static void CreateDirectories(std::string src, std::string dest)
	{
		int length = src.size();
		
		auto all = find_if(nodes.cbegin(), nodes.cend(), [&src](auto d){ return String_::Contains(d->Path(),src);} );
		
		for(auto n : FS::Directory::Nodes())
		{	    
			std::string srcPath = n.Info().Path();
			std::string destPath = srcPath.replace(0,length,dest);
			fs::create_directories(destPath);	
		}			
	}
};

#endif
