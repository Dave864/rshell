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

void GetPut(char ** argv)
{
	ifstream ifs;
	ofstream ofs;
	ifs.open(argv[1]);
	ofs.open(argv[2]);
	char c = ifs.get();
	while(ifs.good())
	{
		if(ofs.good())
		{
			ofs.put(c);
		}
		ifs.get(c);
	}
	ifs.close();
	ofs.close();
}

void ReadWriteChar(char ** argv)
{
	int fd_rd = open(argv[1], O_RDONLY);	
	if(fd_rd == -1)
	{
		perror("open");
		exit(1);
	}
	int fd_wr = open(argv[2], O_WRONLY | O_CREAT);
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
	if(remove(argv[2]) != 0)
	{
		perror("remove");
	}
}

void ReadWriteBuff(char ** argv)
{
	int fd_rd = open(argv[1], O_RDONLY);	
	if(fd_rd == -1)
	{
		perror("open");
		exit(1);
	}
	int fd_wr = open(argv[2], O_WRONLY | O_CREAT);
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
	if(remove(argv[2]) != 0)
	{
		perror("remove");
	}
}

int main(int argc, char ** argv)
{
	if(argc < 4)
	{
		return 0;
	}
	if(strcmp(argv[3], ALL_FLAG) == 0)
	{
		Timer t;
		double etime;
		t.start();

		//run get and put
		GetPut(argv);
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
		ReadWriteChar(argv);
		cout << "Times for read() and write() one character at a time\n";
		t.elapsedWallclockTime(etime);
		cout << "wallclock time: " << etime << endl;
		t.elapsedUserTime(etime);
		cout	<< "user time: " << etime << endl;
		t.elapsedSystemTime(etime);
		cout	<< "system time: " << etime << endl << endl;

		//run read and write via buffer
		t.start();
		ReadWriteChar(argv);
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
	GetPut(argv);
	return 0;
}
