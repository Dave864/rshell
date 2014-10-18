#include <iostream>
#include <string>
#include <string.h>

//creates an array of strings from command with each string being whitespace separated characters
void GetArray(char *& command)
{
	char* elmt; 
	for(int i = 0; command[i] != '\0'; i++)
	{
		//keeps going until non-whitespace is found
		if(!isspace(command[i]))
		{
			int sz = i;
			int strt = i;
			for(; (command[i] != '\0') && !isspace(command[i]); i++);
			sz = i-sz;
			elmt = new char[sz+1];
			for(int j = 0; j < sz; j++)
			{
				elmt[j] = command[strt+j];
			}
			elmt[sz] = '\0';
			std::cout << elmt << std::endl;
		}
	}
}

void Parse(char *& word, const char *& connector)
{
	char* command = strtok(word, connector);
	while(command != NULL)
	{
		std::cout << command << std::endl;
		//run execvp on command
		command = strtok(NULL, connector);
	}
	return;
}

//tells if c is in word
bool FindC(char *& word, const char * c)
{
	unsigned int c_sz;
	for(c_sz = 0; c[c_sz] != '\0'; c_sz++); 
	for(unsigned int i = 0; word[i+c_sz-1] != '\0' ; i++)
	{
		if(c[0] == word[i])
		{
			for(unsigned int j = 1; j < c_sz; j++)
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
	}
	else if(FindC(command, "||"))
	{
		//set token as "||" and parse through command
		Parse(command, or_Op);
	}
	else if(FindC(command, "&&"))
	{
		//set token as "&&" and parse through command
		Parse(command, and_Op);
	}
	//run execvp on command
	GetArray(command);
	delete[] c_str_in;
	return 0;
}
