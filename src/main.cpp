#include <iostream>
#include <string.h>

char ** parse(string & command)
{
	int sz = 0;
	for(int i = 0; i < command.size(); i++)
	{
		sz++;
		if(isspace(command[i]))
		{
			
		}
	}
}

int main()
{
	std::cout << "$ ";
	std::string command;
	getline(std::cin, command);
	return 0;
}
