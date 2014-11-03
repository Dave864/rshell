CFLAGS=-g -Wall -Werror -pedantic -ansi
BIN="bin"

all:
	mkdir -p $(BIN)
	cd bin; g++ ../src/main.cpp $(CFLAGS) -o rshell
	cd bin; g++ ../src/ls.cpp $(CFLAGS) -o ls
	cd bin; g++ ../src/cp.cpp $(CFLAGS) -o cp.out

ls:
	mkdir -p $(BIN)
	cd bin; g++ ../src/ls.cpp $(CFLAGS) -o ls

cp:
	mkdir -p $(BIN)
	cd bin; g++ ../src/cp.cpp $(CFLAGS) -o cp.out

rshell:
	mkdir -p $(BIN)
	cd bin ;g++ ../src/main.cpp $(CFLAGS) -o rshell

clean:
	cd bin; if [ -a rshell ] ;then rm rshell; fi
	cd bin; if [ -a ls ] ; then rm ls; fi
	cd bin; if [ -a cp.out] ; then rm cp.out; fi
	rmdir bin
