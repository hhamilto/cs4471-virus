all: virus host tstWrappers

virus:	virus.c util.c
	gcc -Wall -o virus virus.c 

open: open.c util.c
	gcc -c open.c

tstWrappers: open tstWrappers.c
	gcc -o tstWrappers tstWrappers.c open.o

host:	host.c
	gcc -Wall -o host host.c

PHONY: clean

clean:
	rm -rf virus host tstWrappers