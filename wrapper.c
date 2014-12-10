#include <sys/syscall.h>
// #include <asm/unistd.h>
#include <sys/types.h>
#include <unistd.h>

int open(const char *pathname, int flags, mode_t mode){
  printf("New open being used");
  return(syscall(__NR_open,pathname, flags, mode));
}