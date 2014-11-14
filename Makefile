CFLAGS=-g -Wall -Werror -pedantic -ansi
BIN="bin"

all: src/printLs.h src/my_queue.h
	mkdir -p $(BIN)
	cd bin; if [ -a ../src/rshell.cpp ] ; \
		then g++ ../src/rshell.cpp $(CFLAGS) -o rshell; fi
	cd bin; g++ ../src/main.cpp $(CFLAGS) -o old_rshell
	cd bin; g++ ../src/ls.cpp $(CFLAGS) -o ls

ls: src/printLs.h src/my_queue.h
	mkdir -p $(BIN)
	cd bin; g++ ../src/ls.cpp $(CFLAGS) -o ls

cp:
	mkdir -p $(BIN)
	cd bin; g++ ../src/cp.cpp $(CFLAGS) -o cp

rshell:
	mkdir -p $(BIN)
	cd bin; g++ ../src/rshell.cpp $(CFLAGS) -o rshell

old_rshell:
	mkdir -p $(BIN)
	cd bin; g++ ../src/main.cpp $(CFLAGS) -o old_rshell

clean:
	cd bin; if [ -a rshell ] ; then rm rshell; fi
	cd bin; if [ -a old_rshell ] ; then rm old_rshell; fi
	cd bin; if [ -a ls ] ; then rm ls; fi
	cd bin; if [ -a cp ] ; then rm cp; fi
	rmdir bin
