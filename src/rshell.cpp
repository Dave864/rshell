#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "my_queue.h"
using namespace std;

#define C_NEXT ";"
#define C_AND "&&"
#define C_OR "||"
#define COMMENT "#"
#define RD_IN "<"
#define RD_OUT ">"
#define RD_OUTAPP ">>"
#define PIPE "|"

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

//puts elmts into command
void Insert(My_queue& elmts, char* command[])
{
	int sz = elmts.size();
	for(int i = 0; i < sz; i++)
	{
		command[i] = new char[BUFSIZ];
		strcpy(command[i], elmts.front());
		elmts.pop();
	}
}

//extracts all whitespace seperated substrings from input puts them into
//an array of char*
void GetCom(const char* input, char* command[])
{
	My_queue elmts;
	char tmp[BUFSIZ];
	memset(tmp, '\0', BUFSIZ);
	strcpy(tmp, input);
	char* elmt = strtok(tmp, " \t\n\v\f\r");
	while(elmt != NULL)
	{
		elmts.push(elmt);
		elmt = strtok(NULL, " \t\n\v\f\r");
	}
	Insert(elmts, command);
}

//executes command and returns if it did so
bool Execute(char* command[])
{
	int status;
	pid_t PID;
	PID = fork()
	//fork failed
	if(PID == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	//child process
	else if(PID == 0)
	{
	}
	else
	{
		PID = wait(&status);
		if(PID == -1)
		{
			perror("wait");
			exit(EXIT_FAILURE);
		}
	}
	return false;
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
		//parse input and run each command
	}
	else if(input.find(C_AND) != string::npos)
	{
		if(input.find(C_AND) == 0)
		{
			cerr << "error: connector as first argument\n";
			return;
		}
		//parse input and run command if previous also ran
	}
	else if(input.find(C_OR) != string::npos)
	{
		if(input.find(C_OR) == 0)
		{
			cerr << "error: connector as first argument\n";
			return;
		}
		//parse input and run command if previous failed
	}
	else
	{
		char* command[BUFSIZ];
		memset(command, '\0', BUFSIZ);
		GetCom(input.c_str(), command);
		//free up memory allocated to command
		for(int j = 0; command[j] != '\0';j++)
		{
			delete command[j];
		}
	}
}

int main()
{
	//declare all variables to be used in the rshell loop here
	string input;
	while(1)
	{
		cout << "$ ";
		cin >> ws;
		getline(cin, input);
		DelComment(input);
		if(ExitFound(input))
		{
			return 0;
		}
		RunWCon(input);
	}
	return 0;
}
