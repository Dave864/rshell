##RSHELL

####General Overview

An implementation of the bash shell. When executed, the terminal will display
	
	$

at which point the user will be able to enter commands to run. After each command is run
the prompt will be displayed again until the command

	$ exit

is entered.

######things to note
* This program supports the next (;), and (&&), and or (||) connectors. 
* This program is capable of limited input output redirection.

####Compiling and Running

To compile this program specifically use the command

	make rshell

To run the command after it has been compiled run the command

	bin/rshell

####Bug List

* If multiple types of connectors are entered, the program will run the first command and then
yield an error.
* This program cannot run the cd command 
* When redirecting output to multiple files, only the last file in the chain will recieve
the output. In other words

```
$ command > file1 > file2 > file3
//creates three files
//file1 and file2 are empty
//file3 contains the result of command
```

* When redirecting input multiple times, only the last file in the chain will be read from while
the rest will be empty, similar to the issue before.
* The program is not able to handle the piping of commands.

##LS

####General Overview

Runs similarly to the unix command ls. The program displays the contents for the
current directory or the contents of files and directoires passed in to the program
through the command line. Is also able to run the -l, -R, and -a flag functionalities
when they are passed through the command line.

####Compiling and Running

To compile specifically this program use the command

	make ls

To run the command after it has been compiled run the command

	bin/ls #optional: files -flags

####Bug List

* Does not display the total disk space allocated when the -l flag is invoked.
* When displaying the contents of a file, the output is arranged into columns whose
width is that of the size of the longest name, regardless if the output would occupy
more than one line.
* When the sizes of the file names are large, the names may sometimes overflow into the
next line.
* If an argument does not exist, the program will not run, even if any other argument passed in
does exist.
* If only files are passed in as arguments, calling both the -l and -R flags will not display the
long lis format.
* Directories are not displayed with forward slashes.
* There are numerous inconsistencies in the output format between the ls command and this program.
