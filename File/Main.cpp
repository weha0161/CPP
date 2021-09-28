#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <unordered_map>
// #include "Directory.hpp"
#include "Repository.hpp"
#include "FileSystem.hpp"

using namespace FS;

int main()
{

    const fs::path pathToShow{ "//home//markus//Dokumente//cpp//File" };
//     const fs::path pathToShow{ "//home//markus//Dokumente//cpp//Matrix" };

	auto nodes = FileSystem::List(pathToShow);
	auto root = fs::directory_entry(pathToShow);
	FS::DirectoryInfo* dir = new FS::DirectoryInfo(root.path(),root.last_write_time(),nodes);
	
	nodes.push_back(dir);
	
// 	for(auto it = nodes.cbegin(); it != nodes.cend(); ++it)
// 	{
// 		Logger::Log<Debug>()<<*it<<" "<<(*it)->Path()<<std::endl;
// 	}
	
	Backup::Repository::Map(nodes.cbegin(), nodes.cend());
	
	FileSystem::CreateDirectories("/home/markus/Dokumente/cpp/File","/home/markus/Downloads/");
	Backup::Repository::List();
	Backup::Repository::CopyTo("/home/markus/Downloads/");
	
// 	FileSystem::CreateDirectories("/home/markus/Dokumente/cpp/Matrix","/home/markus/Downloads/");
// 	Backup::Repository::List();
// 	Backup::Repository::CopyTo("/home/markus/Downloads/");
	
    return 0;
};
