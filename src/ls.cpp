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

//runs ls on dirName, implementing any specified flags
void runLS(bool* flags, char* dirName)
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
		if(!flags[0])
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
void	runOnWhich(bool* flags, int argc, char** argv)
{
	char dirName[] = ".";
	//runs ls on specified files in argc
	if(argc > 1)
	{
		for(int i = 1; i < argc; i++)
		{
			//FIXME:create filename
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
	//0 -a
	//1 -l
	//2 -R
	bool flags[3] = {false, false, false};
	checkFlags(flags, argc, argv);
	runOnWhich(flags, argc, argv);
	return 0;
}
