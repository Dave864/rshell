#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
using namespace std;

//checks argc for any flags and sets the appropriate index in flags if so
void checkFlags(bool* flags, int argc, char** argv)
{
	for(int i = 1; i < argc; i++)
	{
		if(argv[i][0] == '-')
		{
			for(int j = 1; argv[i][j] != '\0'; j++)
			{
				//check for -a
				flags[0] = (argv[i][j] == 'a') ? true : flags[0];
				//check for -l
				flags[1] = (argv[i][j] == 'l') ? true : flags[1];
				//check for -R
				flags[2] = (argv[i][j] == 'R') ? true : flags[2];
			}
		}
	}
	return;
}

//runs the ls command on the files in argc, implementing any specified optional flags
void	runOnDir(bool* flags, int argc, char** argv)
{
	char dirName[] = ".";
	//runs ls on specified files in argc
	if(argc > 1)
	{
		for(int i = 1; i < argc; i++)
		{
			//FIXME:create filename
		}
	}
	//runs ls on all files in current directory
	else
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
			cout << direntp->d_name << endl;
		}
		if(errno != 0)
		{
			perror("readdir");
			exit(1);
		}
	}
	return;
}

int main(int argc, char** argv)
{
	//indicates if a flag is set
	//0 -a
	//1 -l
	//2 -R
	bool flags[3] = {false, false, false};
	checkFlags(flags, argc, argv);
	runOnDir(flags, argc, argv);
	return 0;
}
