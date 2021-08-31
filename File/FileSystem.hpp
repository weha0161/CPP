#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <memory>
#include "Directory.hpp"

#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

class FileSystem
{
public:
// 	static std::vector<FS::Info*> nodes;
	/*
	static FileSystem()
	{
		nodes = new std::vector<FS::Info*>();
	}
	*/
	static std::vector<FS::Info*> List(const fs::path& pathToScan) {
		auto nodes = std::vector<FS::Info*>();
		for (const auto& entry : fs::directory_iterator(pathToScan)) {
			const auto filenameStr = entry.path().filename().string();
			if (entry.is_directory()) {
				auto dirnodes = FileSystem::List(entry.path());
				FS::Info* dir = new FS::DirectoryInfo(entry.path(),entry.last_write_time(),dirnodes);
				nodes.push_back(dir);
			}
			else
			{
				FS::Info* file = new FS::FileInfo(entry.path(), entry.last_write_time(), entry.file_size());
				nodes.push_back(file);
// 				Logger::Log<Debug>()<<"Create: "<<file<<std::endl;
			}
		}
		
		return nodes;
	}
};

#endif
