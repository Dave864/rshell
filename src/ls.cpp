#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
using namespace std;

#define FLAG_A 1
#define FLAG_R 2
#define FLAG_L 4

//checks argc for any flags and sets the appropriate index in flags if so
void checkFlags(int & flags, int argc, char** argv)
{
	for(int i = 1; i < argc; i++)
	{
		if(argv[i][0] == '-')
		{
			for(int j = 1; argv[i][j] != '\0'; j++)
			{
				//check for -a
				flags = (argv[i][j] == 'a') ? flags|FLAG_A: flags|0;
				//check for -l
				flags = (argv[i][j] == 'l') ? flags|FLAG_R: flags|0;
				//check for -R
				flags = (argv[i][j] == 'R') ? flags|FLAG_L: flags|0;
			}
		}
	}
	return;
}

//adds destination to the end of the path
void addPath(char* path, char* destination)
{
	//does not add destination if it is a flag
	if(destination[0] == '-')
	{
		return;
	}
	//get sizes of path and destination
	int dest_sz = 0;
	int pth_sz = 0;
	for(int i = 0; destination[i] != '\0'; i++)
	{
		dest_sz ++;
	}
	for(int i = 0; path[i] != '\0'; i++)
	{
		pth_sz++;
	}
	//create path from old_pth then append a '/' at the end 
	//followed by destination
	char * old_pth = path;
	path = new char[pth_sz + dest_sz + 2];
	strcpy(path, old_pth);
	delete[] old_pth;
	path[pth_sz+1] = '/';
	for(int i = 0; i < dest_sz; i++)
	{
		path[pth_sz + 1 + i] = destination[i];
	}
	path[pth_sz + dest_sz + 1] = '\0';
	return;
}

//runs ls on dirName, implementing any specified flags
void runLS(int flags, char* dirName)
{
	DIR* dirp = opendir(dirName);
	if(dirp == NULL)
	{
		perror("opendir");
		exit(1);
	}
	dirent* direntp;
	errno = 0;
	while((direntp = readdir(dirp)) != NULL)
	{
		//if flag -a isn't set, ignore hidden files
		if(!flags & FLAG_A)
		{
			if(direntp->d_name[0] != '.')
			{
				cout << direntp->d_name << endl;
			}
		}
		else
		{
			cout << direntp->d_name << endl;
		}
	}
	if(errno != 0)
	{
		perror("readdir");
		exit(1);
	}
	if(closedir(dirp) == -1)
	{
		perror("closedir");
		exit(1);
	}
	return;
}

//determines which files to run ls on
void	runOnWhich(int flags, int argc, char** argv)
{
	char dirName[] = ".";
	//runs ls on specified files in argc
	if(argc > 1)
	{
		for(int i = 1; i < argc; i++)
		{
			addPath(dirName, argv[i]);
			runLS(flags, dirName);
		}
	}
	//runs ls on all files in current directory
	else
	{
		runLS(flags, dirName);
	}
	return;
}

int main(int argc, char** argv)
{
	//indicates if a flag is set
	//1 -a
	//2 -l
	//4 -R
	int flags = 0;
	checkFlags(flags, argc, argv);
	runOnWhich(flags, argc, argv);
	return 0;
}
