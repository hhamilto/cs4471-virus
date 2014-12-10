all: virus host

virus:	virus.c
	gcc -o virus virus.c

host:	host.c
	gcc -o host host.c

PHONY: clean

clean:
	rm -rf virus host
