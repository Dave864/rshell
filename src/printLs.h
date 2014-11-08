#include <iostream>
using namespace std;

#define BUF_WIDTH 80

struct Long_list
{
	char* mode;
	int hard_lnk;
	char* grp;
	char* usr;
	char* date;
	char* name;
	Long_list(int lnk = 0):mode(), hard_lnk(lnk), grp(), usr(), date(), name(){}
};

struct Dir
{
	string name;
	Long_list* info;
	Dir* subdir;
	Dir* next;
	Dir(string n):name(n), info(NULL), subdir(NULL), next(NULL){}
};

struct To_Print
{
	string name;
	bool isDir;
	Long_list* info;
	To_Print* next;
	To_Print(string n):name(n), isDir(false), info(NULL), next(NULL){}
};

class PrintLs
{
	private:
		Dir* first;
		Dir* cur;
		Dir* curSub;
		To_Print* output;

		void SetUpOut()
		{
			if(output != NULL)
			{
				return;
			}
			if(first == NULL)
			{
				return;
			}
			cur = first;
			To_Print* tmp;
			output = new To_Print(cur->name);
			output->info = cur->info;
			output->isDir = true;
			tmp = output;
			tmp = tmp->next;
			if(cur->subdir != NULL)
			{
				for(curSub = cur->subdir; curSub->next != NULL; curSub = curSub->next)
				{
					tmp = new To_Print(curSub->name);
					tmp->info = curSub->info;
					tmp = tmp->next;
				}
			}
			for(; cur->next != NULL; cur = cur->next)
			{
				tmp = new To_Print(cur->name);
				tmp->info = cur->info;
				tmp = output;
				tmp = tmp->next;
				if(cur->subdir != NULL)
				{
					for(curSub = cur->subdir; curSub->next != NULL; curSub = curSub->next)
					{
						tmp = new To_Print(curSub->name);
						tmp->info = curSub->info;
						tmp = tmp->next;
					}
				}
			}
		}

		int getColWidth()
		{
			return 0;
		}

		int getLnkWidth()
		{
			return 0;
		}

	public:
		PrintLs():first(NULL), cur(NULL), curSub(NULL), output(NULL){}
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
				curSub->info->mode = mode;
				return;
			}
			if(cur->info == NULL)
			{
				cur->info= new Long_list();
			}
			cur->info->mode = mode;
		}

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
					curSub->info = new Long_list(numL);
				}
				return;
			}
			if(cur->info == NULL)
			{
				cur->info= new Long_list(numL);
			}
		}

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
				curSub->info->date= date;
				return;
			}
			if(cur->info == NULL)
			{
				cur->info= new Long_list();
			}
			cur->info->date = date;
		}

		void addL_name(char* name)
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
				curSub->info->name = name;
				return;
			}
			if(cur->info == NULL)
			{
				cur->info= new Long_list();
			}
			cur->info->name = name;
		}

		void Print()
		{
			SetUpOut();
			//To_Print* tmp = output;
		}

		~PrintLs()
		{
			Dir* tmp;
			while(first->next != NULL)
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
			To_Print* tmpOut;
			while(output->next != NULL)
			{
				tmpOut = output;
				output = output->next;
				delete tmpOut;
			}
			delete first;
			delete output;
		}
};
