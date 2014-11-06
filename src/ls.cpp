#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
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

//adds destination to the end of the path 
void addPath(string& path, char* destination)
{
	//does not add destination if it is a flag
	if(destination[0] == '-')
	{
		return;
	}
	path.append("/");
	path.append(destination);
	return;
}

//checks if file is a flag
bool isFlag(char* file)
{
	if(file[0] == '-')
	{
		return true;
	}
	return false;
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
	struct passwd *pw; 
	errno = 0;
	if((pw = getpwuid(statBuf.st_uid)) == NULL)
	{
		if(errno != 0)
		{
			perror("getwuid");
			exit(1);
		}
	}
	cout << pw->pw_name << ' ';
	//displays group name
	struct group *gr;
	errno = 0;
	if((gr = getgrgid(statBuf.st_gid)) == NULL)
	{
		if(errno != 0)
		{
			perror("getgrgid");
			exit(1);
		}
	}
	cout << gr->gr_name << ' ';
	//displays the size in bytes
	cout << statBuf.st_size << ' ';
	//displays the last modified time
	time_t tm = statBuf.st_mtime;
	struct tm time;
	if(localtime_r(&tm, &time) == NULL)
	{
		perror("localtime");
		exit(1);
	}
	char mod_time[80];
	errno = 0;
	if(strftime(mod_time, sizeof(mod_time), "%b %e %R", &time) == 0)
	{
		if(errno != 0)
		{
			perror("strftime");
			exit(1);
		}
	}
	cout << mod_time << ' ';
	//displays the name of the file
	cout << file;
	return;
}

//recursively displays the contents of all directories and their subdirectories
void runLS_R(int flags, string dirName)
{
	cout << "run ls -R" << endl;
	return;
}

//runs ls on directory dirName, implementing any specified flags
void runOnDir(int flags, string& dirName)
{
	DIR* dirp = opendir(dirName.c_str());
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
	if(!(flags & FLAG_L))
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

//determines whether dirName is a file or directory and calls runOnDir if
//dirName is a directory
void runLS(int flags, string& dirName)
{
	struct stat statBuf;
	if(stat(dirName.c_str(), &statBuf) == -1)
	{
		perror("stat");
		exit(1);
	}
	if(S_ISREG(statBuf.st_mode))
	{
		if(flags & FLAG_L)
		{
			showStat(dirName.c_str());
		}
		else
		{
			cout << dirName << endl;
		}
	}
	else
	{
		runOnDir(flags, dirName);
	}
}

//determines which files to run ls on
void	runOnWhich(int flags, int argc, char** argv)
{
	string curDir= ".";
	string dirName;
	//runs ls on specified files in argc
	if(argc > 1)
	{
		bool allFlags = true;
		for(int i = 1; i < argc; i++)
		{
			if(!isFlag(argv[i]))
			{
				dirName = argv[i];
				if(flags & FLAG_R)
				{
					runLS_R(flags, dirName);
				}
				else
				{
					runLS(flags, dirName);
				}
				allFlags = false;
			}
		}
		if(allFlags)
		{
				if(flags & FLAG_R)
				{
					runLS_R(flags, dirName);
				}
				else
				{
					runLS(flags, dirName);
				}
		}
	}
	//runs ls on all files in current directory
	else
	{
		runLS(flags, curDir);
	}
	return;
}

int main(int argc, char** argv)
{
	int flags = 0;
	checkFlags(flags, argc, argv);
	runOnWhich(flags, argc, argv);
	return 0;
}
