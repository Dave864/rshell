CFLAGS=-g -Wall -Werror -pedantic -ansi
BIN="bin"

all:
	mkdir -p $(BIN)
	cd bin; g++ ../src/main.cpp $(CFLAGS) -o rshell
	cd bin; g++ ../src/ls.cpp $(CFLAGS) -o ls
	cd bin; g++ ../src/cp.cpp $(CFLAGS) -o cp

ls:
	mkdir -p $(BIN)
	cd bin; g++ ../src/ls.cpp $(CFLAGS) -o ls

cp:
	mkdir -p $(BIN)
	cd bin; g++ ../src/cp.cpp $(CFLAGS) -o cp

rshell:
	mkdir -p $(BIN)
	cd bin; g++ ../src/main.cpp $(CFLAGS) -o rshell

clean:
	cd bin; if [ -a rshell ] ; then rm rshell; fi
	cd bin; if [ -a ls ] ; then rm ls; fi
	cd bin; if [ -a cp ] ; then rm cp; fi
	rmdir bin
