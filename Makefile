all: virus host

virus:	virus.c
	gcc -Wall -o virus virus.c
	gcc -c open.c
	gcc -o tstWrappers tstWrappers.c open.o


host:	host.c
	gcc -Wall -o host host.c

PHONY: clean

clean:
	rm -rf virus host
