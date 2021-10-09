#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <unordered_map>
#include <boost/mpl/vector.hpp>
#include <boost/filesystem.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/placeholders.hpp>
#include <filesystem>
#include "../Logger/Logger.hpp"
#include "Directory.h"

namespace FS
{
	
	Info::Info(){  };
	~Info::Info(){ };
	Info::Info(std::filesystem::path p, std::uintmax_t s, std::filesystem::file_time_type lm): fs_path(p), name(p.filename()), path(p), size(s), lastModification(lm){  };
	
	FileInfo::FileInfo(){};
	~FileInfo::FileInfo(){};
	FileInfo::FileInfo(std::filesystem::path p, std::filesystem::file_time_type lm, std::uintmax_t s):Info(p,s,lm), extension(p.extension())
		{
			
		};
	
	DirectoryInfo::DirectoryInfo(){};
	~DirectoryInfo::DirectoryInfo(){};
	DirectoryInfo::DirectoryInfo(std::filesystem::path p, std::filesystem::file_time_type lm, std::vector<Info*> n):Info(p,0,lm), nodes(n)
		{
			this->size = this->Size();
		};
}

