#include <iostream>
#include <string>

int main()
{
	std::cout << "$ ";
	std::string command;
	getline(std::cin, command);
	std::cout << command << std::endl;
	return 0;
}
