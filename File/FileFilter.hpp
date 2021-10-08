#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <unordered_map>
#include <boost/filesystem.hpp>
#include <filesystem>
#include "Logger.h"
#include "Directory.h"

#ifndef FILEFILTER_h
#define FILEFILTER_h

struct CPP
{
    static constexpr char Pattern[] = "*.cpp";
};

template<typename T>
class FileFilter
{
public:
    static std::vector<File> Filter(std::vector<File> files)
    {
        std::vector<File> result;
        for_each(files.begin(), files.end(),[&](auto file)
        {
            if(file.extension == T::Pattern)
                result.push_back(file);
        });
        
        return result;
    }
};

#endif
