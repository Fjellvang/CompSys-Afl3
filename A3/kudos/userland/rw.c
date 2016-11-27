/*
 * Halt the system from userland.
 */

#include "lib.h"


size_t buf = 64;

int main(void) {
 //syscall_write(1,(void *)malloc(buf),63);
 char buf3[] = "skriv dit navn\n";
 syscall_write(1, buf3, strlen(buf3));
 char buf5[20];
 buf5[19] = '\0';
 syscall_read(0, buf5, 20);
 syscall_write(1, buf5, 20);
 printf("\n");
 return 0;

 //printf("Hello World\n");
 //syscall_read(0,(void *)malloc(buf),strlen(buf)); 
 //printf("you said %s\n", buf);
 //syscall_halt();
}
