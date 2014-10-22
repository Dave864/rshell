#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

//Increases the size of com_array by 1 and appends to_add and a NULL pointer
//at the end respectively
void expand_array(char **& com_array, char *& to_add, std::vector<int>& Sizes)
{
	char ** tmp;
	tmp = com_array;
	com_array = new char*[Sizes.size()+1];
	for(unsigned int i = 0; i < Sizes.size(); i++)
	{
		com_array[i] = new char[Sizes[i]];
		if(tmp[i] != NULL)
		{
			strcpy(com_array[i], tmp[i]);
		}
	}
	strcpy(com_array[Sizes.size()-1], to_add);
	com_array[Sizes.size()] = NULL;
}

//create an array of strings from command with each string being whitespace 
//separated characters
void GetArray(char **& com_array,char *& command)
{
	char* elmt; 
	com_array = new char*[1];
	com_array[0] = NULL;
	std::vector<int> Sizes(0);
	for(int i = 0; command[i] != '\0'; i++)
	{
		if(!isspace(command[i]))
		{
			//extracts the characters into elmt until the 
			//next whitespace
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
			Sizes.push_back(sz+1);
			expand_array(com_array, elmt, Sizes);
			if(command[i] == '\0')
			{
				break;
			}
		}
	}
}

//runs command in the command and returns whether it executed or not
int RunCom(char *& command)
{
	char ** com_array;
	GetArray(com_array, command);
	/*for(int i = 0; com_array[i] != NULL; i++)
	{
		std::cout << com_array[i] << std::endl;
	}*/
	//create a new process which runs execvp and wait for it to end
	int status;
	pid_t PID;
	PID = fork();
	if(PID == -1)
	{
		perror("fork");
	}
	else if(PID == 0)
	{
		if(execvp(com_array[0], com_array) == -1)
		{
			perror("execvp");
		}
		exit(1);
	}
	else
	{
		PID = wait(&status);
		if(PID == -1)
		{
			perror("wait");
		}
		if(WIFEXITED(status))
		{
			return (WEXITSTATUS(status));
		}
	}
	return 1;
}

//Goes through word, extracting commands seperated by connector, and runs execvp
//on each command as it is extracted
void Parse(char *& word, const char *& connector)
{
	char* command = strtok(word, connector);
	bool failed = true;
	while(command != NULL)
	{
		failed = RunCom(command);
		if(strcmp(connector, "||") == 0)
		{
			if(!failed)
			{
				return;
			}
		}
		else if(strcmp(connector, "&&") == 0)
		{
			if(failed)
			{
				return;
			}
		}
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
	while(1)
	{
		//recieves the input from the user
		std::cout << "$ ";
		std::string input;
		getline(std::cin, input);

		//Checks to if there are any comments and ignores them if they exist
		if(input[input.find_first_not_of(" \t\n\v\f\r")] == '#')
		{
			//Execute execvp with an empty array
			continue;
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
		else
		{
			RunCom(command);
		}
		delete[] c_str_in;
	}
	return 0;
}
