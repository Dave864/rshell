#include <iostream>
#include <iomanip>
#include <math.h>
#include <string.h>
using namespace std;

#define BUF_WIDTH 80

//holds information that is displayed when the -l flag is called in ls
struct Long_list
{
	char mode[11];
	int hard_lnk;
	char* usr;
	char* grp;
	int sze;
	char date[80];
	Long_list():hard_lnk(0), usr(), grp(), sze(0), date() 
	{
		memset(mode, '\0', 11);
		memset(date, '\0', 80);
	}
};

struct Dir
{
	string name;
	Long_list* info;
	Dir* subdir;
	Dir* next;
	Dir(string n):name(n), info(NULL), subdir(NULL), next(NULL){}
};

class PrintLs
{
	private:
		Dir* first;
		Dir* cur;
		Dir* curSub;

		//get column width of all files in current directory
		int subDirColWdth(Dir* lookHere)
		{
			if(lookHere->subdir == NULL)
			{
				return 0;
			}
			unsigned int len = 0;
			Dir* tmp;
			for(tmp = lookHere->subdir; tmp != NULL; tmp = tmp->next)
			{
				len = (strlen(tmp->name.c_str()) > len) ? strlen(tmp->name.c_str()): len;
			}
			return len;
		}

		//get column width of list of command line arguments
		int dirColWdth()
		{
			unsigned int len = 0;
			for(cur = first; cur->next != NULL; cur = cur->next)
			{
				if(cur->subdir == NULL)
				{
					len = (strlen(cur->name.c_str()) > len) ? strlen(cur->name.c_str()): len;
				}
			}
			return len;
		}

		//get length of largest size from files in current directory
		int subDirLnkWdth(Dir* lookHere)
		{
			if(lookHere->subdir == NULL)
			{
				return 0;
			}
			unsigned int len = 0;
			unsigned int cnt;
			Dir* tmp;
			for(tmp = lookHere->subdir; tmp != NULL; tmp = tmp->next)
			{
				cnt = static_cast<int>(log10(tmp->info->sze)+1);
				len = (cnt > len) ? cnt: len;
			}
			return len;
		}

		//get length of largest size from command line arguments
		int dirLnkWdth()
		{
			unsigned int len = 0;
			unsigned int cnt;
			for(cur = first; cur->next != NULL; cur = cur->next)
			{
				if(cur->subdir == NULL)
				{
					cnt = static_cast<int>(log10(cur->info->sze)+1);
					len = (cnt > len) ? cnt: len;
				}
			}
			return len;
		}

		//obatains info needed for Print to format the output
		void formatInfo(bool& pLongList, int& lnkWdth, int& colWdth, Dir* out, bool multCom)
		{
				if(out->subdir == NULL)
				{
					pLongList = (out->info != NULL) ? true: false;
				}
				else
				{
					pLongList = (out->subdir->info != NULL) ? true: false;
				}
				if(pLongList)
				{
					lnkWdth = (multCom) ? dirLnkWdth() :subDirLnkWdth(out); 
				}
				else
				{
					colWdth = (multCom) ? dirColWdth() :subDirColWdth(out);
				}
		}

		//helper function for Print
		void printHelper(Dir* out, int colWdth, int colNum, int& curCol, bool pLongList, int lnkWdth)
		{
			//print in long list format
			if(pLongList)
			{
				cout << out->info->mode << ' '
					<< out->info->hard_lnk << ' '
					<< out->info->usr << ' '
					<< out->info->grp << ' '
					<< setw(lnkWdth) << right << out->info->sze << ' '
					<< out->info->date << ' '
					<< out->name << endl;
			}
			//print just the file names
			else
			{
				if(colNum == 0)
				{
					cout << out->name << "  ";
				}
				else
				{
					if(curCol == colNum)
					{
						cout << endl;
						curCol = 0;
					}
					cout.width(colWdth+1);
					cout << left << out->name << setfill(' ') << "  ";
					curCol++;
				}
			}
		}

	public:
		PrintLs():first(NULL), cur(NULL), curSub(NULL){}
		void addDir(string& dir)
		{
			if(first == NULL)
			{
				first = new Dir(dir);
				cur = first;
				return;
			}
			cur->next = new Dir(dir);
			cur = cur->next;
			curSub = NULL;
		}

		void addSubDir(char* subDir)
		{
			if(cur->subdir == NULL)
			{
				cur->subdir = new Dir(subDir);
				curSub = cur->subdir;
				return;
			}
			curSub->next = new Dir(subDir);
			curSub = curSub->next;
		}

		//add mode long list info to current node
		void addL_mode(char* mode)
		{
			if(first == NULL)
			{
				return;
			}
			if(curSub != NULL)
			{
				if(curSub->info == NULL)
				{
					curSub->info = new Long_list();
				}
				strcpy(curSub->info->mode, mode);
				return;
			}
			if(cur->info == NULL)
			{
				cur->info= new Long_list();
			}
			strcpy(cur->info->mode, mode);
		}

		//add number of links long list info to current node
		void addL_link(int numL)
		{
			if(first == NULL)
			{
				return;
			}
			if(curSub != NULL)
			{
				if(curSub->info == NULL)
				{
					curSub->info = new Long_list();
				}
				curSub->info->hard_lnk = numL;
				return;
			}
			if(cur->info == NULL)
			{
				cur->info= new Long_list();
				cur->info->hard_lnk = numL;
				return;
			}
			cur->info->hard_lnk = numL;
		}

		//add group name long list info to current node
		void addL_grp(char* grp)
		{
			if(first == NULL)
			{
				return;
			}
			if(curSub != NULL)
			{
				if(curSub->info == NULL)
				{
					curSub->info = new Long_list();
				}
				curSub->info->grp= grp;
				return;
			}
			if(cur->info == NULL)
			{
				cur->info= new Long_list();
			}
			cur->info->grp = grp;
		}

		//add user name long list info to current node
		void addL_usr(char* usr)
		{
			if(first == NULL)
			{
				return;
			}
			if(curSub != NULL)
			{
				if(curSub->info == NULL)
				{
					curSub->info = new Long_list();
				}
				curSub->info->usr = usr;
				return;
			}
			if(cur->info == NULL)
			{
				cur->info= new Long_list();
			}
			cur->info->usr = usr;
		}

		//add size long list info to crrent node
		void addL_sze(int size)
		{
			if(first == NULL)
			{
				return;
			}
			if(curSub != NULL)
			{
				if(curSub->info == NULL)
				{
					curSub->info = new Long_list();
				}
				curSub->info->sze = size;
				return;
			}
			if(cur->info == NULL)
			{
				cur->info= new Long_list();
			}
			cur->info->sze = size;
		}

		//add date long list info to current node
		void addL_date(char* date)
		{
			if(first == NULL)
			{
				return;
			}
			if(curSub != NULL)
			{
				if(curSub->info == NULL)
				{
					curSub->info = new Long_list();
				}
				strcpy(curSub->info->date, date);
				return;
			}
			if(cur->info == NULL)
			{
				cur->info= new Long_list();
			}
			strcpy(cur->info->date, date);
		}

		void Print()
		{
			int lnkWdth = 0;
			int colWdth = 0;
			Dir* out = first;
			//Run this if up to one non-flag argument was passed
			if(out->next == NULL)
			{
				bool pLongList; 
				formatInfo(pLongList, lnkWdth, colWdth, out, false);
				int colNum = (colWdth == 0) ? 0: BUF_WIDTH/colWdth;
				int curCol = 0;
				//Run this if out is a directory
				if(out->subdir != NULL)
				{
					for(out = out->subdir; out != NULL; out = out->next)
					{
						printHelper(out, colNum, colWdth, curCol, pLongList, lnkWdth);
					}
				}
				//run this if out is a file
				else
				{
					printHelper(out, colNum, colWdth, curCol, pLongList, lnkWdth);
				}
				if(!pLongList)
				{
					cout << endl;
				}
			}
			//Run this if two or more non-flag arguments passed
			else
			{
				bool pLongList; 
				formatInfo(pLongList, lnkWdth, colWdth, out, true);
				int colNum = (colWdth == 0) ? 0: BUF_WIDTH/colWdth;
				int curCol = 0;
				//find and print out files
				for(; out != NULL; out = out->next)
				{
					if(out->subdir == NULL)
					{
						printHelper(out, colNum, colWdth, curCol, pLongList, lnkWdth);
					}
				}
				//find and print out contents of subdirectories
				for(out = first; out != NULL; out = out->next)
				{
					if(out->subdir != NULL)
					{
						cout << endl << out->name << ":\n";
						for(Dir* tmp = out->subdir; tmp != NULL; tmp = tmp->next)
						{
							printHelper(tmp, colNum, colWdth, curCol, pLongList, lnkWdth);
						}
					}
				}
				if(!pLongList)
				{
					cout << endl;
				}
			}
		}

		~PrintLs()
		{
			Dir* tmp;
			while(first != NULL)
			{
				while(first->subdir != NULL)
				{
					curSub = first->subdir;
					first->subdir = curSub->next;
					delete curSub->info;
					delete curSub;
				}
				tmp = first;
				first = first->next;
				delete tmp->info;
				delete tmp;
			}
		}
};
