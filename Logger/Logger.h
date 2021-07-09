#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <unordered_map>

#ifndef LOGGER_H
#define LOGGER_H

struct Debug//: LogLevelBase
{
    static void Log(std::string message){std::cout<<"[DEBUG]: "<<message<<std::endl;};
};

class Logger
{   
    private:
        std::string logFileName;
        std::ofstream file;
        
        Logger()
        {
            std::cout<<"Logger started"<<std::endl;
            this->logFileName = "Logging.txt";
            this->file.close();
        //     this->file.clear();
            this->file.open(this->logFileName);
            if(!this->file.good())
                throw std::ios_base::failure(this->logFileName);
        }
        Logger(const Logger&) = delete;
        void operator=(const Logger&) = delete;
        
        virtual ~Logger()
        {  
            this->file.close();
        };
    public: 
        static Logger& Instance()
        {
            static Logger logger;
            return logger;
        };

        template<class LogPolicy = Debug>
        void Log(const std::string message)
        {
            if(!this->file.good())
                throw std::ios_base::failure(this->logFileName);
            LogPolicy::Log(message);
            this->file<<message<<std::endl;
        };
};

// template<class LogPolicy = DebugPolicy>
// void Logger::SetLogFile(const std::string fileName)
// {
//     this->logFileName = fileName;
// };

// template<class LogPolicy = DebugPolicy>
// const std::string& Logger::GetLogFileName() const
// {
//     return this->logFileName;
// };


#endif
