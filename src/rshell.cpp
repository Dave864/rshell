#include <iostream>
#include <string>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "redir.h"
using namespace std;

#define C_NEXT ";"
#define C_AND "&&"
#define C_OR "||"
#define COMMENT "#"

//removes comments from input
void DelComment(string& input)
{
	input = input.substr(0, input.find(COMMENT));
}

//returns true if exit was called
bool ExitFound(string input)
{
	if(input.find("exit") == string::npos)
	{
		return false;
	}
	return true;
}

//executes commands normally and returns if it was successful
bool ExecuteNorm(const char* command)
{
	char* comArray[BUFSIZ];
	memset(comArray, '\0', BUFSIZ);
	GetCom(command, comArray);
	int status;
	pid_t PID;
	PID = fork();
	//fork failed
	if(PID == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	//child process
	else if(PID == 0)
	{
	//Change so PATH searching is used
	//============================================
		if(execvp(comArray[0], comArray) == -1)
		{
			perror("execvp");
			exit(EXIT_FAILURE);
		}
	//============================================
	}
	//parent process
	else
	{
		PID = wait(&status);
		if(PID == -1)
		{
			perror("wait");
			exit(EXIT_FAILURE);
		}
		if(WIFEXITED(status))
		{
			if(WEXITSTATUS(status) > 0)
			{
				return false;
			}
			return true;
		}
	}
	return false;
}

//determines whether to execute command with redirection or normally
//and returns if execution was succesful
bool Execute(const char* command)
{
	string com_str = command;
	if(FirstRD(com_str) > -1)
	{
		//piping and/or redirection
		return ExecuteRedir(com_str);
	}
	else
	{
		return ExecuteNorm(command);
	}
	return false;
}

//gets the commands seperated by the connectors and executes each command
void ParseExecute(string input, const char* cnctr)
{
	char* command, *saveptr;
	bool success;
	char tmp[BUFSIZ];
	TokSet(tmp, input);
	command = strtok_r(tmp, cnctr, &saveptr);
	//if command has a connector that isn't cnctr, display error and return
	while(command != NULL)
	{
		//check for redirection and run if it exists
		success = Execute(command);
		if(strncmp(cnctr, C_AND, strlen(cnctr)) == 0)
		{
			if(!success)
			{
				return;
			}
		}
		else if(strncmp(cnctr, C_AND, strlen(cnctr)) == 0)
		{
			if(success)
			{
				return;
			}
		}
		command = strtok_r(NULL, cnctr, &saveptr);
		//if command has a connector that isn't cnctr, display error and return
	}
}

//sees which connector was invoked and executes the input using that connector
void RunWCon(string input)
{
	if(input.find(C_NEXT) != string::npos)
	{
		if(input.find(C_NEXT) == 0)
		{
			cerr << "error: connector as first argument\n";
			return;
		}
		ParseExecute(input, C_NEXT);
	}
	else if(input.find(C_AND) != string::npos)
	{
		if(input.find(C_AND) == 0)
		{
			cerr << "error: connector as first argument\n";
			return;
		}
		ParseExecute(input, C_AND);
	}
	else if(input.find(C_OR) != string::npos)
	{
		if(input.find(C_OR) == 0)
		{
			cerr << "error: connector as first argument\n";
			return;
		}
		ParseExecute(input, C_OR);
	}
	else
	{
		Execute(input.c_str());
	}
}

int main()
{
	string input;
	while(1)
	{
		cout << "$ ";
		getline(cin, input);
		if(input != "\n")
		{
			DelComment(input);
			if(ExitFound(input))
			{
				return 0;
			}
			RunWCon(input);
		}
	}
	return 0;
}
