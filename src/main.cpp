#include <iostream>
#include <string>
#include <string.h>

void Parse(char *& word, const char *& connector)
{
	char * command = strtok(word, connector);
	std::cout << command << std::endl;
	return;
}

//Tells if c is in word
bool FindC(char *& word, const char * c)
{
	for(unsigned int i = 0; i < sizeof(word-sizeof(c)+1); i++)
	{
		if(c[0] == word[i])
		{
			for(unsigned int j = 1; j < sizeof(word); j++)
			{
				if(c[j] != word[i+j])
				{
					break;
				}
			}
			return true;
		}
	}
	return false;
}

int main()
{
	//recieves the input from the user
	std::cout << "$ ";
	std::string input;
	getline(std::cin, input);

	//Checks to if there are any comments and ignores them if they exist
	if(input[input.find_first_not_of(" \t\n\v\f\r")] == '#')
	{
		//Execute execvp with an empty array
		return 0;
	}
	char * c_str_in = new char [input.size()+1];
	strcpy(c_str_in, input.c_str()); 
	char * command = strtok(c_str_in, "#");

	//determines if there are any connectors and sets them as the token
	//if one exists
	const char * sem_Col = ";";
	const char * or_Op = "||";
	const char * and_Op = "&&";
	if(FindC(command, ";"))
	{
		//set token as ';' and parse through command
		Parse(command, sem_Col);
		std::cout << "connector is a ;\n";
	}
	else if(FindC(command, "||"))
	{
		//set token as "||" and parse through command
		Parse(command, or_Op);
		std::cout << "connector is a ||\n";
	}
	else if(FindC(command, "&&"))
	{
		//set token as "&&" and parse through command
		Parse(command, and_Op);
		std::cout << "connector is a &&\n";
	}
	//run execvp on command
	delete[] c_str_in;
	return 0;
}
