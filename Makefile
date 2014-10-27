CFLAGS=-g -Wall -Werror -pedantic -ansi
BIN="bin"

all:
	mkdir -p $(BIN)
	cd bin; g++ ../src/main.cpp $(CFLAGS) -o rshell
	cd bin; g++ ../src/ls.cpp $(CFLAGS) -o ls

ls:
	mkdir -p $(BIN)
	cd bin; g++ ../src/ls.cpp $(CFLAGS) -o ls

rshell:
	mkdir -p $(BIN)
	cd bin ;g++ ../src/main.cpp $(CFLAGS) -o rshell

clean:
	cd bin; rm rshell ls
	rmdir bin
