#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <unordered_map>

bool Is(char c){ return c == '/'; }
bool Non(char c) { return !Is(c);}

std::vector<std::string> split(const std::string s)
{
	using iter = std::string::const_iterator;
	std::vector<std::string> v;
	
	iter i = s.cbegin();
	while (i != s.cend())
	{
		i = find_if(i,s.cend(), Non);
		iter j = find_if(i,s.cend(), Is);
	
		if (i != s.cend())
		{
			v.push_back(std::string(i,j));
		}
		
		i = j;
	}
	
	return v;
};

// int main()
// {
//     const std::string pathToShow{ "/home/markus/Dokumente/cpp/File" };
// 
//     auto c = split(pathToShow);
//     
//     for(auto d : c)
//     	std::cout<<d<<std::endl;
// 	
//     return 0;
// };
