#include <iostream>
#include <string>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
using namespace std;

#ifndef __REDIR_H__
#define __REDIR_H__
#define RD_IN "<"
#define RD_OUT ">"
#define RD_OUTAPP ">>"
#define PIPE "|"

//sets up tmp buffer for use in strtok
void TokSet(char* tmp, string input)
{
	memset(tmp, '\0', BUFSIZ);
	strcpy(tmp, input.c_str());
}

//extracts all whitespace seperated substrings from input puts them into
//an array of char*
void GetCom(const char *input, char *command[])
{
	char tmp[BUFSIZ];
	TokSet(tmp, input);
	char *elmt = strtok(tmp, " \t\n\v\f\r");
	int i = 0;
	while(elmt != NULL)
	{
		command[i] = elmt;
		i++;
		elmt = strtok(NULL, " \t\n\v\f\r");
	}
}

//runs cd command and return if successful or not
bool CD(char *command[])
{
	char *home;
	if((home = getenv("HOME")) == NULL)
	{
		perror("getenv");
		return false;
	}
	int i;
	for(i = 0; command[i] != NULL; i++);
	//go back to home directory if zero or more than one
	//destinatio is given
	if((i == 1) || (i > 2))
	{
		if(chdir(home) == -1)
		{
			perror("chdir");
			return false;
		}
	}
	//go to directory command[1]
	else
	{		
		if(strcmp(command[1], "~") == 0)
		{
			if(chdir(home) == -1)
			{
				perror("chdir");
				return false;
			}
		}
		else
		{
			if(chdir(command[1]) == -1)
			{
				perror("chdir");
				return false;
			}
		}
	}
	return true;
}

//this function does nothing but lets me create an empty char** that isn't
//explicitly used for anything
void KeepMe(char *wordList[])
{
	return;
}

//searches path variable for command and executes it if found
void ExecCom(char *argList[])
{
	//creating this empty char** somehow preserves the contents of argList
	char *empty[BUFSIZ];
	memset(empty, '\0', BUFSIZ);
	KeepMe(empty);
	//get PATH environment variable
	char *path= getenv("PATH");
	if(path == NULL)
	{
		perror("getenv");
		exit(EXIT_FAILURE);
	}
	//searches PATH for command argList[0]
	char tmp[BUFSIZ];
	char *saveptr;
	string com_str;
	TokSet(tmp, string(path));
	char *command = strtok_r(tmp, ":", &saveptr);
	while(command != NULL)
	{
		com_str = command;
		com_str += (com_str[com_str.size()-1] != '/') ? "/": "";
		com_str += argList[0];
		//execute comand argList[0] if it's in path com_str
		if(execv(com_str.c_str(), argList) == -1);
		else
		{
			exit(EXIT_SUCCESS);
		}
		command = strtok_r(NULL, ":", &saveptr);
	}
	perror("execv");
	exit(EXIT_FAILURE);
	return;
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
	char *toAdd;
	char *saveptr;
	char tokBuf[BUFSIZ];
	TokSet(tokBuf, command);
	//separates commands to be piped
	toAdd = strtok_r(tokBuf, PIPE, &saveptr);
	while(toAdd != NULL)
	{
		toRedr.push_back(toAdd);
		toAdd = strtok_r(NULL, PIPE, &saveptr);
	}
	int PID, status;
	int pfd[2];
	for(unsigned int i = 0; i < toRedr.size(); i++)
	{
		if(i != toRedr.size()-1)
		{
			if(pipe(pfd) == -1)
			{
				perror("pipe");
				return false;
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
				if(dup2(pfd[0], 0) == -1)
				{
					perror("dup2");
					exit(EXIT_FAILURE);
				}
			}
			//pipe stdout
			if(i != (toRedr.size()-1))
			{
				if(dup2(pfd[1], 1) == -1)
				{
					perror("dup2");
					exit(EXIT_FAILURE);
				}
			}
			/*if(i == toRedr.size()-1)
			{
				if(close(pfd[0]) == -1)
				{
					perror("close");
				}
				if(close(pfd[1]) == -1)
				{
					perror("close");
				}
			}*/
			//run io redirection
			string toRedr_str = toRedr[i];
			if((i == 0) && (i == toRedr.size()-1))
			{
				Redirect(toRedr_str);
			}
			//execute command in toRedr_str
			char *comArray[BUFSIZ];
			memset(comArray, '\0', BUFSIZ);
			GetCom(toRedr_str.c_str(), comArray);
			ExecCom(comArray);
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
#endif

