CFLAGS=-g -Wall -Werror -pedantic -ansi
BIN="bin"

all: src/printLs.h src/my_queue.h src/redir.h
	mkdir -p $(BIN)
	cd bin; g++ ../src/rshell.cpp $(CFLAGS) -o rshell
	cd bin; g++ ../src/cp.cpp $(CFLAGS) -o cp
	cd bin; g++ ../src/ls.cpp $(CFLAGS) -o ls

ls: src/printLs.h src/my_queue.h
	mkdir -p $(BIN)
	cd bin; g++ ../src/ls.cpp $(CFLAGS) -o ls

cp:
	mkdir -p $(BIN)
	cd bin; g++ ../src/cp.cpp $(CFLAGS) -o cp

rshell: src/redir.h
	mkdir -p $(BIN)
	cd bin; g++ ../src/rshell.cpp $(CFLAGS) -o rshell

clean:
	cd bin; if [ -a rshell ] ; then rm rshell; fi
	cd bin; if [ -a ls ] ; then rm ls; fi
	cd bin; if [ -a cp ] ; then rm cp; fi
	rmdir bin
