#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char** argv){
	int fileFd; 
	int i;
	struct stat fileStat;

	//call open on file
	fileFd = open(argv[1], O_RDWR);//O_RDONLY);

	if(fileFd == -1){
		printf("open returned -1\n");
		return 1;
	}

	//call fstat
	if(fstat(fileFd,&fileStat) !=0){
		printf("No file\n");
		return 1;
	}

	//print the file size an 1st 8 bites in hex
	printf("file size: %d\n", fileStat.st_size);

	printf("1st eight bytes in hex: ");
	for(i = 0; i < 8; i++){
		char buf;
		if(read(fileFd, &buf,1)==-1){perror("read error: ");}
		printf("%x", buf);
	}
	printf("\n");

	// call close
	close(fileFd);

	return 0;
}