#include <iostream>
#include <string>
#include <stdio.h>
#include <errno.h>
using namespace std;

#define C_NEXT ";"
#define C_AND "&&"
#define C_OR "||"
#define COMMENT "#"
#define RD_IN "<"
#define RD_OUT ">"
#define RD_OUTAPP ">>"
#define PIPE "|"

//removes comments from input
void DelComment(string& input)
{
	input = input.substr(0, input.find(COMMENT));
}

//Return true if exit was called
bool ExitFound(string input)
{
	if(input.find("exit") == string::npos)
	{
		return false;
	}
	return true;
}

int main()
{
	//declare all variables to be used in the rshell loop here
	string input;
	while(1)
	{
		getline(cin, input);
		DelComment(input);
		if(ExitFound(input))
		{
			return 0;
		}
		cout << input << endl;
	}
	return 0;
}
