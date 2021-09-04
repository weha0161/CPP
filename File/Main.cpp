#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <unordered_map>
#include "Directory.hpp"
// #include "Visitor.h"
#include "FileSystem.hpp"

using namespace FS;

int main()
{

// 	 
//     const fs::path pathToShow{ "//home//markus//Dokumente//cpp" };
    const fs::path pathToShow{ "//home//markus//Dokumente//cpp//File" };
//     FS::Directory dir = FS::Directory("");

	auto nodes = FileSystem::List(pathToShow);
    
	for(auto it = nodes.cbegin(); it != nodes.cend(); ++it)
		Logger::Log<Debug>()<<*it<<std::endl;
	
	auto n = nodes[3];
	
	using fileTypes = Typelist<CPP,HPP,H,CSV>::Type;
	
// 	Parse<fileTypes>((FileInfo*)n);
	
// 	auto fi = FS::FileInfo();
// 	auto t = FS::TreeParserVisitor();
// 	t.Visit(fi);
	
    return 0;
};
