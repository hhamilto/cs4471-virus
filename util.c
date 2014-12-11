#ifndef CS4471_VIRUS_UTIL
#define CS4471_VIRUS_UTIL 1

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>

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

#endif