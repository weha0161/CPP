#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <unordered_map>
#include "ParseTypes.hpp"
#include "Repository.hpp"
#include "FileSystem.hpp"

using namespace FS;
//----------------------------------------------------------------------------PRODUCT----------------------------------------------------------
// int main()
// {
// 	
// 	std::vector<std::string> dirs= 
// 	{
// 		"/home/markus/Dokumente/cpp/Matrix",
// 		"/home/markus/Dokumente/cpp/File",
// 		"/home/markus/Dokumente/cpp/Visitor",
// 		"/home/markus/Dokumente/cpp/Unit",
// 		"/home/markus/Dokumente/cpp/Wrapper",
// 		"/home/markus/Dokumente/cpp/Traits",
// 		"/home/markus/Dokumente/cpp/SFINAE",
// 		"/home/markus/Dokumente/cpp/Quantity",
// 		"/home/markus/Dokumente/cpp/Logger",
// 		"/home/markus/Dokumente/cpp/Fraction",
// 		"/home/markus/Dokumente/cpp/CSV",
// 		"/home/markus/Dokumente/cpp/COW",
// 		"/home/markus/Dokumente/cpp/AbstractFactory",
// 		"/home/markus/Dokumente/cpp/Typelist",
// 		"/home/markus/Dokumente/cpp/String",
// 	};
// 	
// // 	std::string to ="/media/markus/8591-1355/CPP/";
// 	std::string to ="/home/markus/Downloads/";
// 	
// 	for(auto p : dirs)
// 	{
// 		std::string from = p;
// 		Backup::Repository::Backup(from,to);
// 	}
// 		
//     return 0;
// };

//----------------------------------------------------------------------------TEST----------------------------------------------------------


int main()
{

	
	std::vector<std::string> dirs= 
	{
		"/home/markus/Dokumente/cpp/Matrix",
		"/home/markus/Dokumente/cpp/File",
		"/home/markus/Dokumente/cpp/Visitor",
		"/home/markus/Dokumente/cpp/Unit",
		"/home/markus/Dokumente/cpp/Wrapper",
		"/home/markus/Dokumente/cpp/Traits",
		"/home/markus/Dokumente/cpp/SFINAE",
		"/home/markus/Dokumente/cpp/Quantity",
		"/home/markus/Dokumente/cpp/Logger",
		"/home/markus/Dokumente/cpp/Fraction",
		"/home/markus/Dokumente/cpp/CSV",
		"/home/markus/Dokumente/cpp/COW",
		"/home/markus/Dokumente/cpp/AbstractFactory",
		"/home/markus/Dokumente/cpp/Typelist",
		"/home/markus/Dokumente/cpp/String",
	};
	
	std::string to ="/home/markus/Downloads/";
	for(auto p : dirs)
	{
		std::string from = p;
		auto nodes = FileSystem::List(from);

		auto root = fs::directory_entry(from);
		auto dir = new FS::DirectoryInfo(root.path(),root.last_write_time(),nodes);
				
		nodes.push_back(dir);
		Backup::Repository::Map(nodes.cbegin(), nodes.cend());
		
	}
	
	std::string fileName = "Unit.h";
	std::string csv = "RaibaKonten2021.csv";
	
	auto f = Backup::Repository::Read(fileName);
	
	auto p = Backup::Repository::Parse<FS::Code>(fileName);
	
	for(auto l : p)
		Logger::Log() << l<< std::endl;
	
	auto cv = Backup::Repository::Read(csv);
	
	auto se = Backup::Repository::Parse<FS::AccountTransaction>(csv);
	
	int i = 0;
// 	for(auto i = se.cbegin(); i != se.cend(); ++i)
// 		std::cout<< ++i<<": " <<i->second<< std::endl;
	
	auto keys = se.Keys();
	for (auto elem : keys)
        std::cout << elem.Value << std::endl;
	
	std::cout<<"\n";
	
	for (auto elem : se[keys[1]])
        std::cout << elem << std::endl;
	
    return 0;
};
