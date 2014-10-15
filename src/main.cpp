#include <iostream>
#include <string.h>

int main()
{
	std::cout << "$ ";
	std::string command;
	getline(std::cin, command);
	std::cout << command << std::endl;
	return 0;
}
