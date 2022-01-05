#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <unordered_map>

#ifndef LOGGER_HPP
#define LOGGER_HPP

struct Debug//: LogLevelBase
{
    static constexpr const char* Identifier ="[DEBUG]: ";
};

struct Error//: LogLevelBase
{
    static constexpr const char* Identifier ="[ERROR]: ";
};

class Logger
{   public:
        std::ofstream file;
        std::ostream& out = std::cout;
    private:
        std::string logFileName;
        
        Logger()
        {
            out<<"Logger started"<<std::endl;
//             this->logFileName = "Logging.txt";
//             this->file.close();
        //     this->file.clear();
//             this->file.open(this->logFileName);
//             if(!this->file.good())
//                 throw std::ios_base::failure(this->logFileName);
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
        static std::ostream& Log()
        {
//             if(!this->file.good())
//                 throw std::ios_base::failure(this->logFileName);
			
			return Logger::Instance().out<<LogPolicy::Identifier;
			
//             this->file<<message<<std::endl;
        };
		
        template<typename Iterator,class LogPolicy = Debug>
        static void Log(Iterator begin, Iterator end, std::string name = "")
        {
			int j = 0;

			if(name != "")
				Logger::Log()<<name<<" :"<<std::endl;
			
			for(auto i = begin; i != end; ++i, ++j)
				Logger::Log<LogPolicy>()<<"\t"<<j<<" :"<<*i<<std::endl;
		};
};

// template<class A>
// std::ostream& operator<<(std::ostream& out, const Logger& logger)
// {
// 	return m.Display(out);
// }

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
