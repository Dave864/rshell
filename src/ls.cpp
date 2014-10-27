#include <iostream>

//checks argc for any flags and sets the appropriate index in flags if so
void checkFlags(bool* flags, int argc, char** argv)
{
	return;
}

int main(int argc, char** argv)
{
	//indicates if a flag is set
	//0 -a
	//1 -1
	//2 -R
	bool flags[3] = {false, false, false};
	checkFlags(flags, argc, argv);
	return 0;
}
