#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <unordered_map>
#include "Directory.hpp"
#include "FileSystem.hpp"

int main()
{

    const fs::path pathToShow{ "//home//markus//Dokumente//cpp" };
//     FS::Directory dir = FS::Directory("");

	auto nodes = FileSystem::List(pathToShow);
    
	for(auto it = nodes.cbegin(); it != nodes.cend(); ++it)
		Logger::Log<Debug>()<<*it<<std::endl;
	
    return 0;
}
