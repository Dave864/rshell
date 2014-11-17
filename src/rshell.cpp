#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
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
	TokSet(tmp, input);
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
		if(execvp(command[0], command) == -1)
		{
			perror("execvp");
			exit(EXIT_FAILURE);
		}
	}
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

//frees up memory used by comArray
void FreeMem(char* comArray[])
{
	for(int i = 0; comArray[i] != '\0'; i++)
	{
		delete comArray[i];
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
//it from piece
string GetFile(string& piece, int pos)
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
	return file;
}

//runs the IO redirection commands and piping
void IORedir(string input)
{
	char* piece, *saveptr;
	int pos, com, std_in, std_out;
	string piece_str, file;
	char tmp[BUFSIZ];
	TokSet(tmp, input);
	piece = strtok_r(tmp, PIPE, &saveptr);
	while(piece != NULL)
	{
		piece_str = piece;
		if((std_in = dup(0)) == -1)
		{
			perror("dup");
			exit(EXIT_FAILURE);
		}
		if((std_out = dup(1)) == -1)
		{
			perror("dup");
			exit(EXIT_FAILURE);
		}
		while((com = FirstRD(piece_str)) != -1)
		{
			switch(com)
			{
				case 0:
					pos = piece_str.find(RD_IN);
					if(piece_str[pos+1] == '\0')
					{
						cerr << "error: no target for input redirection\n";
						return;
					}
					//get file for stdin redirection and remove file from piece_str
					file = GetFile(piece_str, pos+1);
					if(file == "")
					{
						cerr << "error: no target for input redirection\n";
						return;
					}
					//remove RD_IN flag from piece_str for use in execvp
					piece_str.erase(pos, 1);
					/*if(close(0) == -1)
					{
						perror("close");
						return;
						//exit(EXIT_FAILURE);
					}
					*///redirect stdin
					break;
				case 1:
					pos = piece_str.find(RD_OUT);
					if(piece_str[pos+1] == '\0')
					{
						cerr << "error: no target for output redirection\n";
						return;
					}
					//get file for stdin redirection and remove file from piece_str
					file = GetFile(piece_str, pos+1);
					if(file == "")
					{
						cerr << "error: no target for output redirection\n";
						return;
					}
					//remove RD_OUT flag from piece_str for use in execvp
					piece_str.erase(pos, 1);
					/*if(close(1) == -1)
					{
						perror("close");
						return;
						//exit(EXIT_FAILURE);
					}
					*///redirect stdout
					break;
				case 2:
					pos = piece_str.find(RD_OUTAPP);
					if(piece_str[pos+2] == '\0')
					{
						cerr << "error: no target for output redirection\n";
						return;
					}
					//get file for stdin redirection and remove file from piece_str
					file = GetFile(piece_str, pos+1);
					if(file == "")
					{
						cerr << "error: no target for output redirection\n";
						return;
					}
					//remove RD_OUTAPP flag from piece_str for use in execvp
					piece_str.erase(pos, 2);
					/*if(close(1) == -1)
					{
						perror("close");
						return;
						//exit(EXIT_FAILURE);
					}
					*///redirect stdout
					break;
				default:
					break;
			}
		}
		//run command piece_str
		//restore original stdin and stdout
		if(dup2(std_in, 0) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		if(dup2(std_out, 1) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		if(close(std_in) == -1)
		{
			perror("close");
			exit(EXIT_FAILURE);
		}
		if(close(std_out) == -1)
		{
			perror("close");
			exit(EXIT_FAILURE);
		}
		piece = strtok_r(NULL, PIPE, &saveptr);
	}
}

//gets the commands seperated by the connectors and executes each command
void ParseExecute(string input, const char* cnctr)
{
	char* command, *saveptr;
	char* comArray[BUFSIZ];
	bool success;
	char tmp[BUFSIZ];
	TokSet(tmp, input);
	command = strtok_r(tmp, cnctr, &saveptr);
	while(command != NULL)
	{
		memset(comArray, '\0', BUFSIZ);
		GetCom(command, comArray);
		//check for redirection and run if it exists
		success = Execute(comArray);
		if(strncmp(cnctr, C_NEXT, strlen(cnctr)) == 0)
		{
			FreeMem(comArray);
		}
		else if(strncmp(cnctr, C_AND, strlen(cnctr)) == 0)
		{
			FreeMem(comArray);
			if(!success)
			{
				return;
			}
		}
		else
		{
			FreeMem(comArray);
			if(success)
			{
				return;
			}
		}
		command = strtok_r(NULL, cnctr, &saveptr);
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
		char* command[BUFSIZ];
		memset(command, '\0', BUFSIZ);
		GetCom(input.c_str(), command);
		if(FirstRD(input) > -1)
		{
			IORedir(input);
		}
		else
		{
			Execute(command);
		}
		FreeMem(command);
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
