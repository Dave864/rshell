#include <iostream>
#include <algorithm>
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

//sets up tmp buffer for use in strtok
void TokSet(char* tmp, string input)
{
	memset(tmp, '\0', BUFSIZ);
	strcpy(tmp, input.c_str());
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

//extracts all whitespace seperated substrings from input puts them into
//an array of char*
void GetCom(const char* input, char* command[])
{
	char tmp[BUFSIZ];
	TokSet(tmp, input);
	char* elmt = strtok(tmp, " \t\n\v\f\r");
	int i = 0;
	while(elmt != NULL)
	{
		command[i] = elmt;
		i++;
		elmt = strtok(NULL, " \t\n\v\f\r");
	}
}

//finds out which redirection command appears first
int FirstRD(string command)
{
	int pos = -1, toRet = -1;
	int tmp;
	//check for input redirection
	tmp = command.find(RD_IN);
	if(tmp != -1)
	{
		pos = tmp;
		toRet = 0;
	}
	//check for output redirection
	tmp = command.find(RD_OUT);
	if(tmp != -1)
	{
		if(pos != -1)
		{
			pos = (pos > tmp)? tmp: pos;
			toRet = (pos == tmp)? 1: toRet;
		}
		else
		{
			pos = tmp;
			toRet = (toRet == -1)? 1: toRet;
		}
	}
	//check for output append redirection
	tmp = command.find(RD_OUTAPP);
	if(tmp != -1)
	{
		if(pos != -1)
		{
			pos = (pos > tmp)? tmp: pos;
			if(toRet != 0)
			{
				toRet = (pos != tmp)? 1: 2;
			}
			else
			{
				toRet = (pos == tmp)? 2: toRet;
			}
		}
		else
		{
			pos = tmp;
			toRet = (toRet == -1)? 2: toRet;
		}
	}
	//check for pipe
	tmp = command.find(PIPE);
	if(tmp != -1)
	{
		if(pos != -1)
		{
			pos = (pos > tmp)? tmp: pos;
			toRet = (pos == tmp)? 3: toRet;
			toRet = 3;
		}
		else
		{
			pos = tmp;
			toRet = (toRet == -1)? 3: toRet;
		}
	}
	return toRet;
}

//gets the word immediately after the first io redirection command and removes
//both it and the word from piece
string GetFile(string& piece, int pos, int rdcom_sz = 1)
{
	int end_pos = piece.find_first_not_of(" \t\n\v\f\r", pos);
	unsigned int tmp;
	string file;
	if(end_pos == -1)
	{
		file = "";
		return file;
	}
	for(; !isspace(piece[end_pos]); end_pos++)
	{
		tmp = end_pos;
		if(tmp == piece.size())
		{
			break;
		}
		if(piece.compare(end_pos, 1, RD_IN) == 0)
		{
			break;
		}
		if(piece.compare(end_pos, 1, RD_OUT) == 0)
		{
			break;
		}
		if(piece.compare(end_pos, 2, RD_OUTAPP) == 0)
		{
			break;
		}
	}
	file = piece.substr(pos, end_pos-pos);
	size_t sz = end_pos - pos;
	piece.erase(pos, sz);
	piece.erase(pos-rdcom_sz, rdcom_sz);
	file.erase(0, file.find_first_not_of(" \t\n\v\f\r"));
	return file;
}

//redirects stdin and stdout depending on redirection commands in command
void Redirect(string& command)
{
	int rd_com, pos, fd_in = -1, fd_out = -1;
	string file;
	while((rd_com = FirstRD(command)) > -1)
	{
		switch(rd_com)
		{
			//redirect stdin
			case 0:
				if(fd_in != -1)
				{
					if(close(fd_in) == -1)
					{
						perror("close");
						exit(EXIT_FAILURE);
					}
				}
				else
				{
					if(close(0) == -1)
					{
						perror("close");
						exit(EXIT_FAILURE);
					}
				}
				pos = command.find(RD_IN);
				file = GetFile(command, pos+1);
				if((fd_in = open(file.c_str(), O_CREAT|O_RDONLY, S_IRUSR|S_IWUSR)) == -1)
				{
					perror("open");
					exit(EXIT_FAILURE);
				}
				if(dup(fd_in) == -1)
				{
					perror("dup");
					exit(EXIT_FAILURE);
				}
				break;
			//redirect stdout
			case 1:
				if(fd_out != -1)
				{
					if(close(fd_out) == -1)
					{
						perror("close");
						exit(EXIT_FAILURE);
					}
				}
				else
				{
					if(close(1) == -1)
					{
						perror("close");
						exit(EXIT_FAILURE);
					}
				}
				pos = command.find(RD_OUT);
				file = GetFile(command, pos+1);
				if((fd_out = open(file.c_str(), O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR|S_IWUSR)) == -1)
				{
					perror("open");
					exit(EXIT_FAILURE);
				}
				if(dup(fd_out) == -1)
				{
					perror("dup");
					exit(EXIT_FAILURE);
				}
				break;
			//redirect stdout append
			case 2:
				if(fd_out != -1)
				{
					if(close(fd_out) == -1)
					{
						perror("close");
						exit(EXIT_FAILURE);
					}
				}
				else
				{
					if(close(1) == -1)
					{
						perror("close");
						exit(EXIT_FAILURE);
					}
				}
				pos = command.find(RD_OUTAPP);
				file = GetFile(command, pos+2, 2);
				if((fd_out = open(file.c_str(), O_CREAT|O_WRONLY|O_APPEND, S_IRUSR|S_IWUSR)) == -1)
				{
					perror("open");
					exit(EXIT_FAILURE);
				}
				if(dup(fd_out) == -1)
				{
					perror("dup");
					exit(EXIT_FAILURE);
				}
				break;
			default:
				break;
		}
	}
}

//executes commands while conducting io redirection
bool ExecuteRedir(string command)
{
	vector <char*> toRedr;
	char* toAdd;
	char* saveptr;
	char tokBuf[BUFSIZ];
	TokSet(tokBuf, command);
	//separates commands to be piped
	toAdd = strtok_r(tokBuf, PIPE, &saveptr);
	while(toAdd != NULL)
	{
		toRedr.push_back(toAdd);
		toAdd = strtok_r(NULL, PIPE, &saveptr);
	}
	//pipe from parent to child
	int pp2c[2];
	//pipe from child to parent
	int pc2p[2];
	int PID, status;
	for(unsigned int i = 0; i < toRedr.size(); i++)
	{
		if(i != toRedr.size()-1)
		{
			if(pipe(pp2c) == -1)
			{	
				perror("pipe");
				continue;
			}
			if(pipe(pc2p) == -1)
			{	
				perror("pipe");
				continue;
			}
		}
		PID = fork();
		//fork failed
		if(PID == -1)
		{
			perror("fork");
		}
		//child process
		else if(PID == 0)
		{
			//pipe stdin
			if(i != 0)
			{
				if(close(pp2c[1]) == -1)
				{
					perror("close");
					exit(EXIT_FAILURE);
				}
				if(dup2(pp2c[0], 0) == -1)
				{
					perror("dup2");
					exit(EXIT_FAILURE);
				}
			}
			//pipe stdout
			if(i != (toRedr.size()-1))
			{
				if(close(pc2p[0]) == -1)
				{
					perror("close");
					exit(EXIT_FAILURE);
				}
				if(dup2(pc2p[1], 1) == -1)
				{
					perror("dup");
					exit(EXIT_FAILURE);
				}
			}
			//run io redirection
			string toRedr_str = toRedr[i];
			if((i == 0) && (i == toRedr.size()-1))
			{
				Redirect(toRedr_str);
			}
			//execute command in toRedr_str
			char* comArray[BUFSIZ];
			memset(comArray, '\0', BUFSIZ);
			GetCom(toRedr_str.c_str(), comArray);
			if(execvp(comArray[0], comArray) == -1)
			{
				perror("execvp");
				exit(EXIT_FAILURE);
			}
		}
	}
	for(unsigned j = 0; j < toRedr.size(); j++)
	{
		if(wait(&status) == -1)
		{
			perror("wait");
		}
	}
	return false;
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
		if(execvp(comArray[0], comArray) == -1)
		{
			perror("execvp");
			exit(EXIT_FAILURE);
		}
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
