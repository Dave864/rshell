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
#define FLAG_L 2
#define FLAG_R 4

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
				flags = (argv[i][j] == 'l') ? flags|FLAG_L: flags|0;
				//check for -R
				flags = (argv[i][j] == 'R') ? flags|FLAG_R: flags|0;
			}
		}
	}
	return;
}

//adds destination to the end of the path and returns 
//whether this was done or not
bool addPath(char* path, char* destination)
{
	//does not add destination if it is a flag
	if(destination[0] == '-')
	{
		return false;
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
	return true;
}

//displays additional information about file
void showStat(const char* file)
{
	struct stat statBuf;
	if(stat(file, &statBuf) == -1)
	{
		perror("stat");
		exit(1);
	}
	//displays the permissions of file
	char r, w, x;
	if(S_ISDIR(statBuf.st_mode))
	{
		cout << 'd';
	}
	else if(S_ISLNK(statBuf.st_mode))
	{
		cout << 'l';
	}
	else
	{
		cout << '-';
	}
	r = (S_IRUSR & statBuf.st_mode) ? 'r': '-';
	w = (S_IWUSR & statBuf.st_mode) ? 'w': '-';
	x = (S_IXUSR & statBuf.st_mode) ? 'x': '-';
	cout << r << w << x;
	r = (S_IRGRP & statBuf.st_mode) ? 'r': '-';
	w = (S_IWGRP & statBuf.st_mode) ? 'w': '-';
	x = (S_IXGRP & statBuf.st_mode) ? 'x': '-';
	cout << r << w << x;
	r = (S_IROTH & statBuf.st_mode) ? 'r': '-';
	w = (S_IWOTH & statBuf.st_mode) ? 'w': '-';
	x = (S_IXOTH & statBuf.st_mode) ? 'x': '-';
	cout << r << w << x << ' ';
	//displays number of hard links
	cout << statBuf.st_nlink << ' ';
	//displays owner name
	//displays group name
	//displays the size in bytes
	//displays the last modified time
	cout << file;
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
		//if flag -a is set, display hidden files
		if(flags & FLAG_A)
		{
			if(flags & FLAG_L)
			{
				showStat(direntp->d_name);
				cout << endl;
			}
			else
			{
				//need to output in format similar to ls
				cout << direntp->d_name << "  ";
			}
		}
		else
		{
			if(direntp->d_name[0] != '.')
			{
				if(flags & FLAG_L)
				{
					showStat(direntp->d_name);
					cout << endl;
				}
				else
				{
					//need to output in format similar to ls
					cout << direntp->d_name << "  ";
				}
			}
		}
	}
	if(flags & FLAG_L);
	else
	{
		cout << endl;
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
		bool allFlags = true;
		for(int i = 1; i < argc; i++)
		{
			if(addPath(dirName, argv[i]))
			{
				runLS(flags, dirName);
				allFlags = false;
			}
		}
		if(allFlags)
		{
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
