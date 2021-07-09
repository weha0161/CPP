#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <string>
#include "Logger.h"

// template<class LogPolicy = DebugPolicy>
// Logger::~Logger()
// {
//     this->file.close();
// };
// 
// template<class LogPolicy = DebugPolicy>
// Logger& Logger::Instance()
// {
//     static Logger logger;
//     return logger;
// };
// 
// template<class LogPolicy = DebugPolicy>
// void Logger::SetLogFile(const std::string fileName)
// {
//     this->logFileName = fileName;
// };
// 
// template<class LogPolicy = DebugPolicy>
// void Logger::Log(const std::string message)
// {
//     if(!this->file.good())
//         throw std::ios_base::failure(this->logFileName);
//     std::string logMessage = this->GetLogMode() + " " + message;
//     this->file<<logMessage;
// };
// template<class LogPolicy = DebugPolicy>
// const std::string& Logger::GetLogFileName() const
// {
//     return this->logFileName;
// };
// 
// template<class LogPolicy = DebugPolicy>
// Logger::Logger()
// {
//     std::cout<<"Logger started"<<std::endl;
//     this->logFileName = "Logging.txt";
//     this->file.close();
// //     this->file.clear();
//     this->file.open(this->logFileName);
//     if(!this->file.good())
//         throw std::ios_base::failure(this->logFileName);
// }


// template<class LogPolicy = DebugPolicy>
// Logger& operator<<(Logger& logger, std::string message)
// {
//     logger.Log(message);
//     return logger;
// };

// template<class LogPolicy = DebugPolicy>
// Logger& operator<<(Logger& logger, int value)
// {
//     return logger<<std::to_string(value);
// };
// template<class LogPolicy = DebugPolicy>
// Logger& operator<<(Logger& logger, double value)
// {
//     return logger<<std::to_string(value);
// };
