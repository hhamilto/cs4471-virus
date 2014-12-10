#include <stdlib.h>
#include <stdio.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


int main(int argc, char** argv){
	//I'm the host
	if(argc<2){
		printf("Please run me with a file as my first arg.\n");
	}else{
		struct stat imTrash;
		int ret = stat(argv[1],&imTrash);
		if(ret == -1){
			printf("Please run me with a file as my first arg.\n");
		}else{
			printf("Doing operation on: %s\n", argv[1]);
			printf("Done\n");
		}
	}
	return 0;
}