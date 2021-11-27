#include <iostream>
#include "CommandLineParser.hpp"

int main()
{       
    std::cout<<"START"<<std::endl;
	
	char c;
	auto p = CommandLine::Parser();
	while(std::cin.get(c))
		p.event(c);
	
    std::cout<<"END"<<std::endl;

    return 0;
}
