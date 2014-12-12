all: virus host tstWrappers

virus:	virus.c util.c
	gcc -Wall -o virus virus.c 

open: open.c
	gcc -c open.c

close: close.c
	gcc -c close.c

tstWrappers: open close tstWrappers.c
	gcc -o tstWrappers tstWrappers.c open.o close.o

host:	host.c
	gcc -Wall -o host host.c

PHONY: clean

clean:
	rm -rf virus host tstWrappers open.o close.o