#include <iostream>
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

int main(int argc, char** argv)
{
	//indicates if a flag is set
	//0 -a
	//1 -l
	//2 -R
	bool flags[3] = {false, false, false};
	checkFlags(flags, argc, argv);
	cout << "Flag -a is ";
	if(flags[0])
		cout << "set\n";
	else
		cout << "not set\n";
	cout << "Flag -l is ";
	if(flags[1])
		cout << "set\n";
	else
		cout << "not set\n";
	cout << "Flag -R is ";
	if(flags[2])
		cout << "set\n";
	else
		cout << "not set\n";
	return 0;
}
