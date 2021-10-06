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
/*
    fs::path pathToShow{ "//home//markus//Dokumente//cpp//File" };
    const fs::path pathToShow{ "//home//markus//Dokumente//cpp//Matrix" };

	auto nodes = FileSystem::List(pathToShow);
	auto root = fs::directory_entry(pathToShow);
	FS::DirectoryInfo* dir = new FS::DirectoryInfo(root.path(),root.last_write_time(),nodes);
	
	nodes.push_back(dir);
	Backup::Repository::Map(nodes.cbegin(), nodes.cend());
	
	FileSystem::CreateDirectories("/home/markus/Dokumente/cpp/File","/home/markus/Downloads/");
	Backup::Repository::List();
	Backup::Repository::CopyTo("/home/markus/Downloads/");
    
	fs::path pathToShow = "//home//markus//Dokumente//cpp//Matrix";

	auto nodes2 = FileSystem::List(pathToShow);
	auto root = fs::directory_entry(pathToShow);
	auto dir = new FS::DirectoryInfo(root.path(),root.last_write_time(),nodes2);
	
	nodes2.push_back(dir);
	Backup::Repository::Map(nodes2.cbegin(), nodes2.cend());*/
	
	FileSystem::CreateDirectories("/home/markus/Dokumente/cpp/Matrix","/home/markus/Downloads/");
	Backup::Repository::List();
	Backup::Repository::CopyTo("/home/markus/Downloads/");
	
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
// 		std::string to ="/media/markus/8591-1355/1/";
		
		Backup::Repository::Backup(from,to);
	}
// 	
// 	std::string from = "/home/markus/Dokumente/cpp";
// 		
// 		Backup::Repository::Backup(from,to);
//     
// 	std::string from2 = "//home//markus//Dokumente//cpp//File";
// 	Backup::Repository::Backup(from2,to);
	
	std::string fileName = "Unit.h";
	
	auto f = Backup::Repository::Read(fileName);
	

	auto p = Backup::Repository::Parse<FS::CODE>(fileName);
	
	for(auto l : p)
		Logger::Log() << l<< std::endl;
	
    return 0;
};
