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
		PrintLs():first(NULL), cur(first), curSub(NULL){}
		void addDir(string& dir);
		void addSubDir(char* subDir);
		void Print();
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
