#include <iostream>
#include <string>
#include <string.h>

int main()
{
	//recieves the input from the user
	std::cout << "$ ";
	std::string command;
	getline(std::cin, command);
	//determines if there are any connectors and sets them as the token
	//if one exists
	int in_of_connector;
	if(command.find(';') > -1)
	{
		//set token as ';' and parse through command
	}
	else if(command.find("||") > -1)
	{
		//set token as "||" and parse through command
	}
	else if(command.find("&&") > -1)
	{
		//set token as "&&" and parse through command
	}
	//run execvp on command
	return 0;
}
