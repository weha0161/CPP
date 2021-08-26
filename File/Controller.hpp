#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <memory>
// #include "File.h"

#ifndef CONTROLLER_H
#define CONTROLLER_H

class Controller
{
    private:
        const std::string filePattern = ".csv";
//         std::unique_ptr<File> file;
        std::vector<std::string> files;
    public:
        Controller();
};

#endif
