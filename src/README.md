#How to Use Strtok

###What is Strtok?

  The `strtok` function lets you take a character array and break it up into
  smaller substrings, with each one being separated by another specified character.

  What this means is that you, the user, can take the word
  ```
  char *example_0 = behold+the+power+of+strtok!;
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

#####Why You Should Care

  When used appropriately, strtok can make string parsing very easy to do, which
  saves you time and sanity.

###Using Strtok

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
  ```
  char *example_1 = Rootabega;
  char *first_token = strtok(example_1, "a");
  ```
  Here we are parsing the string "Rootabega". The deliminator is the letter "a". So we are looking for
  the first substring in "Rootabega" that does not have the letter "a" in it. From this we see that 
  `first_token` will be assigned the word "Root", the first token of the string.
  ```
  char *first_token = strtok(example_1, "a");
  //is the same as
  char *first_token = "Root";
  ```


  This is all well and good but it still does not explain why in the first example, `NULL` was used in
  subsequent uses of `strtok` instead of the string being parsed. What were to happen if we did not do 
  that?
  Look at this program 
  ```
  #include <iostream>
  #include <string.h>

  int main()
  {
  	char *example_2 = "Rootabeganot";
	
	//get the first two tokens
	char *first_token = strtok(example_2, "a");
	char *second_token = strtok(example_2, "a");

	//display the first two tokens
	cout << "Token 1: " << first_token << endl
	     << "Token 2: " << second_token << endl;
  	return 0;
  }
  ```
  Based on what has been learned from how the function is set up one would expect the output to look something
  like this
  ```
  Token 1: Root
  Token 2: beg
  ```
  However when the program is compiled and executed, the output you do get is this
  ```
  Token 1: Root
  Token 2: Root
  ```

  * Look back at example given to see how strtok works
    * brief overview of how strtok works
    * step by step walkthrough of what is going on in example
      * go over what happens if deliminating character is not in the string being parsed
      * go over cases where deliminator is at beginning, end, or both
      * go over what happens when you change deliminator
      * deliminator is literal, it will search for the exact character when extracting tokens
        * There is an execption with whitespace characters, (either single space is interpreted as
	   all, or what I did in my assignments)

#####Parsing Multiple Strings

  * Mention parsing mutiple strings
    * What will happen if midway parsing through one we begin parsing another?

###Enter the _r (Parsing multiple strings)

  * See how strtok_r works
    * note similarity to strtok with addition of saveptr
    * what saveptr does
  * step by step walthrough with new example
    * edge cases and deliminator are same with strtok_r as they are with strtok

###Things to Remember

  Not sure about what to put here yet, might just remove it.

