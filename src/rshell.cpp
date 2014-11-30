#include <iostream>
#include <string>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
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
	//run cd command if it exists
	if(string(comArray[0]) == "cd")
	{
		return CD(comArray);
	}
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
		ExecCom(comArray);
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

//finds the first connector in word
int FirstCon(string word)
{
	int cur = 0, prev = -1, con = 0;
	//find which connector comes first
	if((cur = word.find(C_NEXT)) != -1)
	{
		con = 1;
		prev = cur;
	}
	if((cur = word.find(C_AND)) != -1)
	{
		if((cur < prev) || (prev == -1))
		{
			con = 2;
			prev = cur;
		}
	}
	if((cur = word.find(C_OR)) != -1)
	{
		if((cur < prev) || (prev == -1))
		{
			con = 3;
		}
	}
	return con;
}

//gets the commands seperated by the connectors and executes each command
void ParseExecute(string input, const char* cnctr)
{
	char *command, *saveptr;
	bool success;
	char tmp[BUFSIZ];
	TokSet(tmp, input);
	command = strtok_r(tmp, cnctr, &saveptr);
	//if command has a connector that isn't cnctr, display error and return
	if(FirstCon(string(command)) > 0)
	{
		cerr << "error: more than one connector used\n";
		return;
	}
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
		else if(strncmp(cnctr, C_OR, strlen(cnctr)) == 0)
		{
			if(success)
			{
				return;
			}
		}
		command = strtok_r(NULL, cnctr, &saveptr);
		//if command has a connector that isn't cnctr, display error and return
		if(command == NULL);
		else if(FirstCon(string(command)) > 0)
		{
			cerr << "error: more than one connector used\n";
			return;
		}
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
	char cwd[BUFSIZ];
	while(1)
	{
		if(getcwd(cwd, BUFSIZ) == NULL)
		{
			perror("getcwd");
			exit(EXIT_FAILURE);
		}
		cout << cwd << " $ ";
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
