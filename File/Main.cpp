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
// 	std::string to ="/media/markus/8591-1355/CPP/";
// // 	std::string to ="/home/markus/Downloads/";
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
	std::string custom = "RaibaKonten2021_1.csv";
	std::string comdirect = "Umsaetze_1026947527.csv";
	std::string raiba = "Umsaetze_DE19660623660009232702.csv";
	
	auto f = Backup::Repository::Read(fileName);
	
	auto p = Backup::Repository::Parse<FS::Code>(fileName);
	
	for(auto l : p)
		Logger::Log() << l<< std::endl;
	
	auto cv = Backup::Repository::Read(custom);	
	auto se = Backup::Repository::Parse<FS::Custom<0>>(custom);
	se.Display(std::cout);
	
// 	auto c = Backup::Repository::Read(comdirect);	
// 	auto com = Backup::Repository::Parse<FS::Comdirect<0>>(comdirect);
// 	com.Display(std::cout);
	
	auto c = Backup::Repository::Read(raiba);	
	auto r = Backup::Repository::Parse<FS::Raiba<0>>(raiba);
	r.Display(std::cout);
	
    return 0;
};
