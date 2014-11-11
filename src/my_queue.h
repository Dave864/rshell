#include <iostream>
#include <string.h>
using namespace std;

#ifndef __MY_QUEUE_H__
#define __MY_QUEUE_H__

struct str
{
	char word[BUFSIZ];
	struct str* nxt;
	str(const char* w): nxt(NULL)
	{
		strcpy(word, w);
	}
};

class My_queue
{
	struct str* top;

	public:
		My_queue(): top(NULL){};

		void push(const char* toAdd)
		{
			if(this->empty())
			{
				top = new str(toAdd);
				return;
			}
			for(struct str* tmp = top; tmp != NULL; tmp = tmp->nxt)
			{
				if(tmp->nxt == NULL)
				{
					tmp->nxt = new str(toAdd);
					return;
				}
			}
		}

		void pop()
		{
			if(!this->empty())
			{
				struct str* tmp = top;
				top = top->nxt;
				delete tmp;
			}
		}

		bool empty()
		{
			return (top == NULL);
		}

		const char* front()
		{
			if(this->empty())
			{
				return '\0';
			}
			else
			{
				return top->word;
			}
		}

		~My_queue()
		{
			while(!this->empty())
			{
				this->pop();
			}
			delete top;
		}
};
#endif
