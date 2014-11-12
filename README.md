##RSHELL

####Bug List

* Unable to handle commands chained by connectors
* Unable to run the cd command

##LS

####General Overview

Runs similarly to the unix command ls. The program displays the contents fo the
current directory or the contents of files and directoires passed in to the program
through the command line. Is also able to run the -l, -R, and -a flag functionalities
when they are passed through the command line.

####Compiling and Running

To compile specifically this program use the command

----
make ls
----

To run the command after it has been compiled run the command

----
bin/ls #optional: files -flags
----

####Bug List

* Does not display the total disk space allocated when the -l flag is invoked
* When displaying the contents of a file, the output is arranged into columns whose
width is that of the size of the longest name, regardless if the output would occupy
more than one line.

**When the sizes of the file names are large, the names may sometimes overflow into the
next line.
