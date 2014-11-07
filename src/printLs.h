#include <iostream>
using namespace std;

struct Dir
{
	string name;
	//longlist info
	Dir* subdir;
	Dir* next;
	Dir(string n):name(n), subdir(NULL), next(NULL){}
};

class PrintLs
{
	private:
		Dir* first;
		Dir* cur;
		Dir* curSub;

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

		void Print()
		{
			//if only one Dir node, print subdir if it exists, otherwise, print name
			if(first->next == NULL)
			{
				if(first->subdir == NULL)
				{
					cout << first->name << endl;
				}
				else
				{
					//print out subdirectory
					curSub = first->subdir;
					//add name to array
					while(curSub->next != NULL)
					{
						//add name to array
					}
					//print out the formatted array
				}
				return;
			}
		}
		//void getLongList();
		~PrintLs()
		{
			Dir* tmp;
			while(first->next != NULL)
			{
				while(first->subdir != NULL)
				{
					curSub = first->subdir;
					first->subdir = curSub->next;
					delete curSub;
				}
				tmp = first;
				first = first->next;
				delete tmp;
			}
			delete first;
		}
};
