/*
 * Halt the system from userland.
 */

#include "lib.h"
size_t buf = 64;
int main(void) {
 
 syscall_read(0,(void *)malloc(buf),63); 
 syscall_write(1,(void *)malloc(buf),63);
 syscall_halt();
 return 0;
}
