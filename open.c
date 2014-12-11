#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int open(const char *pathname, int flags, mode_t mode){
  printf("New open being used");
  return(syscall(5,pathname, flags, mode));
}

int close(int fdes){
  printf("New open being used");
  return(syscall(6,fdes));
}


