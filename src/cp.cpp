#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "Timer.h"
using namespace std;

#define ALL_FLAG "-a"

void GetPut(char * source, char * dest)
{
	ifstream ifs;
	ofstream ofs;
	ifs.open(source);
	ofs.open(dest);
	char c = ifs.get();
	while(ifs.is_open() && !ifs.eof())
	{
		if(ofs.is_open())
		{
			ofs.put(c);
		}
		ifs.get(c);
	}
	ifs.close();
	ofs.close();
}

void ReadWriteChar(char * source, char * dest)
{
	int fd_rd = open(source, O_RDONLY);	
	if(fd_rd == -1)
	{
		perror("open");
		exit(1);
	}
	int fd_wr = open(dest, O_WRONLY | O_CREAT);
	if(fd_wr == -1)
	{
		perror("open");
		exit(1);
	}
	char in[1]; 
	int err;
	while((err = read(fd_rd, in, sizeof(char))))
	{
		if(err == -1)
		{
			perror("read");
			exit(1);
		}
		if(write(fd_wr, in, sizeof(char)) == -1)
		{
			perror("write");
			exit(1);
		}
	}
	if(close(fd_rd) == -1)
	{
		perror("close");
		exit(1);
	}
	if(close(fd_wr) == -1)
	{
		perror("close");
		exit(1);
	}
	if(remove(dest) != 0)
	{
		perror("remove");
	}
}

void ReadWriteBuff(char * source, char * dest)
{
	int fd_rd = open(source, O_RDONLY);	
	if(fd_rd == -1)
	{
		perror("open");
		exit(1);
	}
	int fd_wr = open(dest, O_WRONLY | O_CREAT);
	if(fd_wr == -1)
	{
		perror("open");
		exit(1);
	}
	char* in[1]; 
	int err;
	while((err = read(fd_rd, in, sizeof(char*))))
	{
		if(err == -1)
		{
			perror("read");
			exit(1);
		}
		if(write(fd_wr, in, sizeof(char*)) == -1)
		{
			perror("write");
			exit(1);
		}
	}
	if(close(fd_rd) == -1)
	{
		perror("close");
		exit(1);
	}
	if(close(fd_wr) == -1)
	{
		perror("close");
		exit(1);
	}
	if(remove(dest) != 0)
	{
		perror("remove");
	}
}

bool GetFiles(int argc, char** argv, char* source, char* dest)
{
	bool flag = false;
	for(int i = 1; i < argc; i++)
	{
		if(strcmp(argv[i], ALL_FLAG) == 0)
		{
			flag = true;
		}
		else
		{
			if(source[0] == '\0')
			{
				strncpy(source, argv[i], 128);
			}
			if(source != argv[i])
			{
				strncpy(dest, argv[i], 128);
			}
		}
	}
	return flag;
}

//check if a file exists and if it's a directory
bool FileExists(char * dest)
{
	struct stat statBuf;
	if(stat(dest, &statBuf) == 0)
	{
		if(S_ISDIR(statBuf.st_mode))
		{
			cerr << "error: Destination is a directory\n";
		}
		else
		{
			cerr << "Destination already exists\n";
		}
		return true;
	}
	return false;
}

int main(int argc, char ** argv)
{
	if(argc < 3)
	{
		return 0;
	}
	char source[128];
	memset(source, '\0', 128);
	char dest[128];
	memset(dest, '\0', 128);
	if(GetFiles(argc, argv, source, dest))
	{
		if(FileExists(dest))
		{
			return 0;
		}
		Timer t;
		double etime;
		t.start();
		//run get and put
		GetPut(source, dest);
		cout << "Times for in.get(char) and out.put(char)\n";
		t.elapsedWallclockTime(etime);
		cout << "wallclock time: " << etime << endl;
		t.elapsedUserTime(etime);
		cout	<< "user time: " << etime << endl;
		t.elapsedSystemTime(etime);
		cout	<< "system time: " << etime << endl << endl;
		if(remove(argv[2]) != 0)
		{
			perror("remove");
		}
		//run read and write via character
		t.start();
		ReadWriteChar(source, dest);
		cout << "Times for read() and write() one character at a time\n";
		t.elapsedWallclockTime(etime);
		cout << "wallclock time: " << etime << endl;
		t.elapsedUserTime(etime);
		cout	<< "user time: " << etime << endl;
		t.elapsedSystemTime(etime);
		cout	<< "system time: " << etime << endl << endl;

		//run read and write via buffer
		t.start();
		ReadWriteChar(source, dest);
		cout << "Times for read() and write() one buffer at a time\n";
		t.elapsedWallclockTime(etime);
		cout << "wallclock time: " << etime << endl;
		t.elapsedUserTime(etime);
		cout	<< "user time: " << etime << endl;
		t.elapsedSystemTime(etime);
		cout	<< "system time: " << etime << endl;
		return 0;
	}
	//run most efficient one
	if(!FileExists(dest))
	{
		GetPut(source, dest);
	}
	return 0;
}
