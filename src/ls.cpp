#include <iostream>
#include <string>
#include <queue>
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
#include "printLs.h"
#include "my_queue.h"
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
void addPath(string& path, const char* destination)
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
void showStat(const char* file, PrintLs & output)
{
	struct stat statBuf;
	if(stat(file, &statBuf) == -1)
	{
		perror("stat");
		exit(1);
	}
	//gets the permissions of file
	char mode[11];
	memset(mode, '\0', 11);
	if(S_ISDIR(statBuf.st_mode))
	{
		mode[0] = 'd';
	}
	else if(S_ISLNK(statBuf.st_mode))
	{
		mode[0] = 'l';
	}
	else
	{
		mode[0] = '-';
	}
	//user permissions
	mode[1] = (S_IRUSR & statBuf.st_mode) ? 'r': '-';
	mode[2] = (S_IWUSR & statBuf.st_mode) ? 'w': '-';
	mode[3] = (S_IXUSR & statBuf.st_mode) ? 'x': '-';
	//group permissions
	mode[4] = (S_IRGRP & statBuf.st_mode) ? 'r': '-';
	mode[5] = (S_IWGRP & statBuf.st_mode) ? 'w': '-';
	mode[6] = (S_IXGRP & statBuf.st_mode) ? 'x': '-';
	//others' permissions
	mode[7] = (S_IROTH & statBuf.st_mode) ? 'r': '-';
	mode[8] = (S_IWOTH & statBuf.st_mode) ? 'w': '-';
	mode[9] = (S_IXOTH & statBuf.st_mode) ? 'x': '-';
	mode[10] = '\0';
	output.addL_mode(mode);
	//obtains number of hard links
	output.addL_link(statBuf.st_nlink);
	//obtains owner name
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
	output.addL_usr(pw->pw_name);
	//obtains group name
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
	output.addL_grp(gr->gr_name);
	//obtains the size in bytes
	output.addL_sze(statBuf.st_size);
	//obtains the last modified time
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
	output.addL_date(mod_time);
	return;
}

//recursively displays the contents of all directories and their subdirectories
void runLS_R(int flags, string dirName)
{
	My_queue subDir;
	PrintLs output_R;
	output_R.addDir(dirName);
	struct stat statBuf;
	if(stat(dirName.c_str(), &statBuf) == -1)
	{
		perror("stat");
		exit(1);
	}
	if(S_ISDIR(statBuf.st_mode))
	{
		DIR* dirp = opendir(dirName.c_str());
		if(dirp == NULL)
		{
			perror("opendir");
			exit(1);
		}
		string file = dirName;
		dirent* direntp;
		errno = 0;
		while((direntp = readdir(dirp)) != NULL)
		{
			//if flag -a is set, display hidden files
			if(flags & FLAG_A)
			{
				file = direntp->d_name;
				if((file != ".") && (file != ".."))
				{
					file = dirName;
					addPath(file, direntp->d_name);
					if(stat(file.c_str(), &statBuf) == -1)
					{
						perror("stat");
						exit(1);
					}
					if(S_ISDIR(statBuf.st_mode))
					{
						subDir.push(direntp->d_name);
						output_R.addSubDir(direntp->d_name);
						if(flags & FLAG_L)
						{
							showStat(file.c_str(), output_R);
						}
					}
					else
					{
						output_R.addSubDir(direntp->d_name);
						if(flags & FLAG_L)
						{
							showStat(file.c_str(), output_R);
						}
					}
					file = dirName;
				}
			}
			else
			{
				if(direntp->d_name[0] != '.')
				{
					addPath(file, direntp->d_name);
					if(stat(file.c_str(), &statBuf) == -1)
					{
						perror("stat");
						exit(1);
					}
					if(S_ISDIR(statBuf.st_mode))
					{
						subDir.push(direntp->d_name);
						output_R.addSubDir(direntp->d_name);
						if(flags & FLAG_L)
						{
							showStat(file.c_str(), output_R);
						}
					}
					else
					{
						output_R.addSubDir(direntp->d_name);
						if(flags & FLAG_L)
						{
							showStat(file.c_str(), output_R);
						}
					}
					file = dirName;
				}
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
		cout << dirName << ":\n";
		output_R.Print();
		cout << endl;
		for(;!subDir.empty(); subDir.pop())
		{
			//recursive call
			addPath(file, subDir.front());
			runLS_R(flags, file);
			file = dirName;
		}
	}
	else
	{
		output_R.Print();
	}
	return;
}

//runs ls on directory dirName, implementing any specified flags
void runOnDir(int flags, string& dirName, PrintLs & output)
{
	DIR* dirp = opendir(dirName.c_str());
	if(dirp == NULL)
	{
		perror("opendir");
		exit(1);
	}
	string file = dirName;
	dirent* direntp;
	errno = 0;
	while((direntp = readdir(dirp)) != NULL)
	{
		//if flag -a is set, display hidden files
		if(flags & FLAG_A)
		{
			if(flags & FLAG_L)
			{
				addPath(file, direntp->d_name);
				output.addSubDir(direntp->d_name);
				showStat(file.c_str(), output);
				file = dirName;
			}
			else
			{
				output.addSubDir(direntp->d_name);
			}
		}
		else
		{
			if(direntp->d_name[0] != '.')
			{
				if(flags & FLAG_L)
				{
					addPath(file, direntp->d_name);
					output.addSubDir(direntp->d_name);
					showStat(file.c_str(), output);
					file = dirName;
				}
				else
				{
					output.addSubDir(direntp->d_name);
				}
			}
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

//determines whether dirName is a file or directory and calls runOnDir if
//dirName is a directory
void runLS(int flags, string& dirName, PrintLs & output)
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
			showStat(dirName.c_str(), output);
		}
	}
	else
	{
		runOnDir(flags, dirName, output);
	}
}

//determines which files to run ls on
void	runOnWhich(int flags, int argc, char** argv, PrintLs & output)
{
	string curDir = ".";
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
					output.addDir(dirName);//class func call
					runLS(flags, dirName, output);
				}
				allFlags = false;
			}
		}
		//if all arguments are flags, run ls on curDir
		if(allFlags)
		{
				if(flags & FLAG_R)
				{
					runLS_R(flags, curDir);
				}
				else
				{
					output.addDir(curDir);//class func call
					runLS(flags, curDir, output);
				}
		}
	}
	//runs ls on all files in current directory
	else
	{
		output.addDir(curDir);//class func call
		runLS(flags, curDir, output);
	}
	return;
}

int main(int argc, char** argv)
{
	PrintLs output;
	int flags = 0;
	checkFlags(flags, argc, argv);
	runOnWhich(flags, argc, argv, output);
	if(flags & FLAG_R)
	{
	}
	else
	{
		output.Print();
	}
	return 0;
}
