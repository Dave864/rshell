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
	char * c_str_in = new char [input.size()+1];
	strcpy(c_str_in, input.c_str()); 
	char * command = strtok(c_str_in, "#");
	std::cout << command << std::endl;
	delete[] c_str_in;
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
