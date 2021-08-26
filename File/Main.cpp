#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <unordered_map>
// #include "File.h"
#include "Directory.hpp"
// #include "Controller.h"

int main()
{

    const fs::path pathToShow{ fs::current_path() };
    FS::Directory dir = FS::Directory();
//     dir.Map(pathToShow);
    
    return 0;
}
