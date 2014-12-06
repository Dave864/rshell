#How to Use Strtok

###What is Strtok?

  The `strtok` function lets you take a character array and break it up into
  smaller substrings, with each one being separated by another specified character.

  What this means is that you, the user, can take the word
  ```
  char *example = behold+the+power+of+strtok!;
  ```
  and split it up into smaller components that are separated by the character `+` as follows
  ```
  cout << strtok(example, "+") << endl; //behold
  cout << strtok(NULL, "+") << endl;    //the
  cout << strtok(NULL, "+") << endl;    //power
  cout << strtok(NULL, "+") << endl;    //of
  cout << strtok(NULL, "+") << endl;    //strtok
  ```

#####Why You Should Care

  When used appropriately, strtok makes string parsing very easy to do, which saves
  saves you time and sanity.

###Using Strtok

###Enter the _r (strtok_r)

###Things to Remember

