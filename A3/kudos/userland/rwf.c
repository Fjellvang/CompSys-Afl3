
#include "lib.h"


size_t buf = 64;

int main(void) {
  int fd;

  //Error handling
  if(fd = syscall_open(~/Documents/test.txt) < 0){
    char buf1[] = "Couldnt open file. Did you specify it corretly?";
    syscall_write(1, buf1, strlen(buf1));
  }
  //read 20 chars from file
  syscall_read(fd, buf5, strlen(buf5));
  //print to screen  
  syscall_write(1, buf5, strlen(buf5)); 

  char buf2[] = "you can write to the file";
  syscall_write(1, buf2, strlen(buf2));
  char buf3[20];
  buf3[19] = '\0';
  //get input from user
  syscall_read(0, buf3, strlen(buf3));
  //write to file
  syscall_write(fd, buf3, strlen(buf3));
}
