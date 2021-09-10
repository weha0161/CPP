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
		
public:

	static std::vector<FS::Info*> List(const fs::path& pathToScan) {

		for (const auto& entry : fs::directory_iterator(pathToScan)) {
			const auto filenameStr = entry.path().filename().string();
			if (entry.is_directory()) {
				auto dirnodes = FileSystem::List(entry.path());
				FS::DirectoryInfo* dir = new FS::DirectoryInfo(entry.path(),entry.last_write_time(),dirnodes);
				Logger::Log()<<entry.path().string()<<std::endl;
				nodes.push_back(dir);
			}
			else
			{
				Logger::Log()<<entry.path().string()<<std::endl;
				FS::FileInfo* file = new FS::FileInfo(entry.path(), entry.last_write_time(), entry.file_size());
				nodes.push_back(file);
			}
		}
		
		return nodes;
	}
	
	static void CreateDirectories()
	{
		for(auto n : nodes)
		{
			auto c = String_::Split<Delimiter>(n->Path());
	    
			for(auto d : c)
				std::cout<<d<<std::endl;	
		}
	}
};

#endif
