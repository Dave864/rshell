#How to Use Strtok

###What is Strtok?

  The `strtok` function lets you take a character array and break it up into
  smaller substrings, with each one being separated by another specified character.

  What this means is that you, the user, can take the word
  ```
  char example_0[] = "behold+the+power+of+strtok!";
  ```
  and split it up into smaller components that are separated by the character `+`
  ```
  cout << strtok(example_0, "+") << endl;
  cout << strtok(NULL, "+") << endl;
  cout << strtok(NULL, "+") << endl;
  cout << strtok(NULL, "+") << endl;
  cout << strtok(NULL, "+") << endl;
  ```
  to get output like this
  ```
  behold
  the
  power
  of
  strtok!
  ```

####Why You Should Care

  When used appropriately, strtok can make string parsing very easy to do, which
  saves you time and sanity.

###The Way of Strtok
####One Small Step for String(Parsing once)

  The example above does not make it clear exactly how `strtok` is used. Looking at how the function 
  is set up may provide the answer. Accesseing the man page for `strtok` shows the function is set up like
  this.
  ```
  #include <string.h>
  char *strtok(char *str, const char *delim);
  ```
  So the function takes two arguments, the string that is to be parsed, `str`, and what is known as 
  the **delimiter**, `delim`. It returns what is called a **token**, which is the first substring
  in `str` that does not contain `delim`.

  Let's look at another example and break down what is going on

  	char example_1[] = Rootabega;
 	char *first_token = strtok(example_1, "a");

  Here we are parsing the string "Rootabega". The deliminator is the letter "a". So we are looking for
  the first substring in "Rootabega" that does not have the letter "a" in it. From this we see that 
  `first_token` will be assigned the word "Root", the first token of the string.

	char *first_token = strtok(example_1, "a");
	//is the same as
  	char *first_token = "Root";

####Gaze into the NULL (Parsing multiple times)

  We now know the basics of how `strtok` works, but we still do not have an explanation for why in the 
  first example, `NULL` was used instead of the string in subsequent uses of `strtok` when parsing out the 
  next several tokens. What were to happen if we did not do that?
  Look at this program 

	 #include <iostream>
 	 #include <string.h>
	 using namespace std;

 	 int main()
  	{
  		char example_2[] = "Rootabeganot";
	
		//get the first two tokens
		char *first_token = strtok(example_2, "a");
		char *second_token = strtok(example_2, "a");

		//display the first two tokens
		cout << "Token 1: " << first_token << endl
	     	<< "Token 2: " << second_token << endl;
  		return 0;
  	}

  Based on what has been learned from how the function is set up you would expect the output to look like this
  ```
  Token 1: Root
  Token 2: beg
  ```
  However when the program is compiled and executed, the output you get is this
  ```
  Token 1: Root
  Token 2: Root
  ```
  Why does this happen? The answer is because whenever something is passed into the `str` argument for`strtok`,
  the function assumes that this is a new string to parse. The way `strtok` works is that whenever it finds a t
  oken, it removes that token and the following deliminator from the string being looked at. It then holds on to 
  the rest of the string to be use as the "default" if no string is passed in. In the above program, both 
  calls to `strtok` recieved a new string to parse, overriding the default value.

  With this in mind let's change the above program and walk through what is happening.
  ```
	 #include <iostream>
 	 #include <string.h>
	 using namespace std;

 	 int main()
  	{
  		char example_2[] = "Rootabeganot";
	
		//get the first two tokens
		char *first_token = strtok(example_2, "a");
		char *first_copy = strtok(example_2, "a");//changed variable name
		char *second_token = strtok(NULL, "a");   //added this new command

		//display the first two tokens
		cout << "Token 1: " << first_token << endl
	     	<< "Token 2: " << second_token << endl;
  		return 0;
  	}
  ```
  In the first call to `strtok` the first token is found to be "Root". The function then removes the token and
  the delimiter from the `str` argument and saves this new string for later use.
  ```
  char *first_token = strtok(example_2, "a");
  //first_token = "Root"
  //"Roota" is removed from "Rootabeganot" to get "beganot", which is saved for later
  ```
  In the second call to `strtok`, a new value was passed into the `str` argument, overriding the previous saved
  string. The function runs like before; getting the token, removing it and the delimiter and saving the new 
  string for later.
  ```
  char *first_copy = strtok(example_2, "a");
  //a new str argument was given, so the saved string from before, "beganot", is ignored
  //first_copy = "Root"
  //"Roota" is removed from "Rootabeganot" to get "beganot", which is saved for later
  ```
  In the third call to `strtok`, no new argument is given, since `NULL` means nothing, so the previous saved 
  string is used for the `str` argument and the function runs as before.
  ```
  char *second_token = strtok(NULL, "a");
  //NULL, or nothing, is passed in to the str argument
  //find first token from "beganot"
  //second_token = "beg"
  //"bega" is removed from "beganot" to get "not", which is saved for later
  ```
  So we know now why `NULL` is used in strtok. However, looking at this example raises the question of what would
  happen if we wanted to get the third token? This also bring up another question, what would happen if we tried 
  to go beyond that?

####The Last Token and Beyond! (Rules of the delimiter)
  In the previous example we took the string "Rootabeganot" and used the `strtok` function to extract the first
  two tokens of this string using "a" as the delimiter. Looking at what remains after getting the tokens, "Root"
  and "beg", we see that the string still contains "not". Looking at this string, the delimiter "a" is nowhere
  to be found. So what would happen if we ran `strtok` again? What if we ran it after that?

  Let's modify the program from before and find out
  ```
	 #include <iostream>
 	 #include <string.h>
	 using namespace std;

 	 int main()
  	{
  		char example_2[] = "Rootabeganot";
	
		//get the tokens
		char *first_token = strtok(example_2, "a");
		char *second_token = strtok(NULL, "a");
		char *third_token = strtok(NULL, "a");
		char *fourth_token = strtok(NULL, "a");

		//display the tokens
		cout << "Token 1: " << first_token << endl
	     	<< "Token 2: " << second_token << endl
			<< "Token 3: " << third_token << endl
			<< "Token 4: " << fourth_token << endl;
  		return 0;
  	}
  ```
  Running this program gives us the following output
  ```
  Token 1: Root
  Token 2: beg
  Token 3: not
  Token 4:
  ```
  From this we see that `strtok` is able to get the last token, even though the delimiter was not in it, and
  that the function is able to handle the case of an empty string.

  In **One Small Step for String** it was stated that to get the token, `strtok` looks for the first substring
  in the `str` argument that does not contain the deliminator `delim`. So, if the delimiter is not in the `str`
  argument, then the entire argument is considered to be the token.

  Here is another example of this
  ```
  char example_3[] = "This is the entire token";
  char *first_token = strtok(example_3, "A");
  //Look for the first occurence of "A"
  //"A" was not found
  //first_token = "This is the entire token";
  ```
  In the case when there is nothing left to look at, or if nothing was initially passed in to the `str` argument,
  `strtok` returns `NULL`. This enables the use of `while` loops for parsing a string.
  ```
  	#include <iostream>
	#include <string.h>
	using namespace std;

	int main()
	{
		char example_4[] = "See how% the modulo blade%slices % through % this string";
		char *token;
		token = strtok(example_4, "%");
		//prints out each token in example_4
		while(token != NULL)
		{
			cout << token << endl;
			token = strtok(NULL, "%");
		}
		return 0;
	}
	//Output:
	//See how
	// the modulo blade
	//slices 
	// through 
	// this string
  ```
  We know now what happens when the delimiter is not in the `str` argument. What if, however, the delimiter
  were at the beginning and or end of the it? What about if the delimiter was chained together multiple times?

  In the case of the delimiter appearing at the ends of the `str` argument, `strtok` simply ignores them.
  ```
  char example_6[] = "@ What happens@to delimiters@ at the ends@";
  char *token = strtok(example_6, "@");
  //in strtok
  //@ What happens@to delimiters@ at the ends@ 
  //is seen as
  // What happens@to delimiters@ at the ends
  ```
  For the case where the deliminator is chained together multiple times, each instance where that happens is
  the same as if the deliminator was only placed once.
  ```
  char example_7[] = "@@ What @@happens to @ chained @@@@@ delimiters";
  char *token = strtok(example_7, "@");
  //in strtok
  //@@ What @@happens to @ chained @@@@@ delimiters 
  //is the same as
  //@ What @happens to @ chained @ delimiters
  //which is then seen as
  // What @happens to @ chained @ delimiters
  ```

  So far we have only been using examples where the delimiter is only one letter or character. The `strtok`
  function can actually take in multi-character strings as its delimiter. However, when this is done, the
  function doesn't look for the first occurence of the exact character pattern when gettin the token; it looks 
  for the first occurence of any of the characters passed into the `delim` argument.
  ```
	#include <iostream>
	#include <string.h>
	using namespace std;

	int main()
	{
		char example_8[] = "Testing 1 to 2 find 12 many 21 delims 121 in a 212 string";
		char *token;
		token = strtok(example_8, "12");
		//prints out each token in example_4
		while(token != NULL)
		{
			cout << token << endl;
			token = strtok(NULL, "12");
		}
		return 0;
	}
	//Output:
	//Testing 
	// to 
	// find 
	// many 
	// delims
	// in a 
	// string
  ```

  It should also be noted that in subsequent calls to `strtok` when parsing a string, you can change what the
  delimiter is to get different tokens.
  ```
	#include <iostream>
	#include <string.h>
	using namespace std;

	int main()
	{
		char example_9[] = "I have/changed my delimiter?partway through";
		char *token;
		token = strtok(example_9, "/");
		cout << token << endl;
		token = strtok(NULL, " ");
		cout << token << endl;
		token = strtok(NULL, "?");
		cout << token << endl;
		token = strtok(NULL, "+");
		cout << token << endl;
		return 0;
	}
	//Output:
	//I have
	//changed
	//my delimiter
	//partway through
  ```

###The Hidden _r (Parsing multiple strings)

  * Mention parsing mutiple strings
    * What will happen if midway parsing through one we begin parsing another?
  * See how strtok_r works
    * note similarity to strtok with addition of saveptr
    * what saveptr does
  * step by step walkthrough with new example
    * edge cases and deliminator are same with strtok_r as they are with strtok

###Recap (And for those who didn't want to read everything else)

  * `strtok` can be used to parse strings
  * function setup for `strtok`

  ```
  #include <string.h>
  char *strtok(char *str, const char *delim);
  ```
  * when continuing to parse the same string, `str` should be `NULL` in each subsequent call to `strtok`
  * if there is nothing left to parse, `strtok` returns `NULL`
  * rules of `delim`, the delimiter
    * this must always be specified when using `strtok`
    * is ignored when at the beginning or end of the `str` argument
    * `strtok` looks for the first occurrence of any character in `delim`, not the exact pattern that was
    passed in
  * strtok_r is used to parse multiple strings at once
  * function setup for `strtok_r`
  	
  ```
  #include <string.h>
  char *strtok_r(char *str, const char *delim, char **saveptr);
  ```
  * `strtok_r` follows the same rules as `strtok` with the addition of the `saveptr` argument

