#How to Use Strtok

###What is Strtok?

  The `strtok` function lets you take a character array and break it up into
  smaller substrings, with each one being separated by another specified character.

  What this means is that you, the user, can take the word
  ```
  char *example = behold+the+power+of+strtok!;
  ```
  and split it up into smaller components that are separated by the character `+`
  ```
  cout << strtok(example, "+") << endl;
  cout << strtok(NULL, "+") << endl;
  cout << strtok(NULL, "+") << endl;
  cout << strtok(NULL, "+") << endl;
  cout << strtok(NULL, "+") << endl;
  //behold
  //the
  //power
  //of
  //strtok
  ```

#####Why You Should Care

  When used appropriately, strtok can make string parsing very easy to do, which
  saves you time and sanity.

###Using Strtok

  Look back at example given to see how strtok works
  * look at function prototype of strtok
  * brief overview of how strtok works
  * step by step walkthrough of what is going on in example
    * go over cases where deliminator is at beginning, end, or both
    * go over what happens when you change deliminator
    * deliminator is literal, it will search for the exact character when extracting tokens
      * There is an execption with whitespace characters, (either single space is interpreted as
	   all, or what I did in my assignments)

#####Parsing Multiple Strings

  Mention parsing mutiple strings
  * What will happen if midway parsing through one we begin parsing another?

###Enter the _r (Parsing multiple strings)

  See how strtok_r works
  * note similarity to strtok with addition of saveptr
    * what saveptr does
  * step by step walthrough with new example
    * edge cases and deliminator are same with strtok_r as they are with strtok

###Things to Remember

  Not sure about what to put here yet, might just remove it.

