#include <iostream>
#include <string>
#include <string.h>

void parse(std::string & command, const char * connector)
{
	return;
}

int main()
{
	//recieves the input from the user
	std::cout << "$ ";
	std::string input;
	getline(std::cin, input);
	//Checks to if there are any comments and ignores them if they exist
	char * command; 
	//determines if there are any connectors and sets them as the token
	//if one exists
	/*const char * sem_Col = ";";
	const char * or_Op = "||";
	const char * and_Op = "&&";
	if(command.find(';') != std::string::npos)
	{
		//set token as ';' and parse through command
		parse(command, sem_Col);
	}
	else if(command.find("||") != -std::string::npos)
	{
		//set token as "||" and parse through command
		parse(command, or_Op);
	}
	else if(command.find("&&") != std::string::npos) 
	{
		//set token as "&&" and parse through command
		parse(command, and_Op);
	}*/
	//run execvp on command
	return 0;
}
