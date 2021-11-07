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

std::vector<std::string> GetFiles(std::string filename = "Backup.txt")
{
	std::ifstream file(filename);
	std::vector<std::string> result;
	if (file.is_open()) 
	{
		std::string line;
		while (std::getline(file, line)) {
			result.push_back(line);
		}
		file.close();
	}

	return result;
}
using namespace FS;
//----------------------------------------------------------------------------PRODUCT----------------------------------------------------------
int main()
{
	auto dirs = GetFiles();
	
// 	std::string to ="/media/markus/8591-1355/CPP/";
 	std::string to ="/media/markus/Seagate/CPP/";
// 	
	for(auto dir : dirs)
	{
		Backup::Repository::Backup(dir,to);
	}
		
    return 0;
};

//----------------------------------------------------------------------------TEST----------------------------------------------------------
