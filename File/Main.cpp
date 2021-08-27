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

    const fs::path pathToShow{ fs::current_path() };
    FS::Directory dir = FS::Directory("");

	FileSystem::List("//home//markus//Dokumente//cpp//");
    
    return 0;
}
