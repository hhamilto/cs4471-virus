all: virus host

virus:	virus.c
	gcc -Wall -o virus virus.c

host:	host.c
	gcc -Wall -o host host.c

PHONY: clean

clean:
	rm -rf virus host
