#include <iostream>
#include <stdio.h>
#include "Parser.hpp"

int main()
{       
    std::cout<<"START"<<std::endl;
	
	char c;
	auto p = CommandLine::Parser();
// 	while(c = std::cin.get())
// 		p.Event(c);
// 	
	p.Parse();
	std::string s = "END";
// 	std::string a;
//     std::cout<<s;
// 	std::cin>>a;
// 
//     std::cout<<(s+a)<<std::endl;

	std::cout<<"END"<<std::endl;

    return 0;
}
