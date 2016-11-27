/*
 * Halt the system from userland.
 */

size_t buf = 64;
#include "lib.h"
int main(void) {
 
 syscall_read(0,(void *)malloc(buf),63); 
 syscall_write(1,(void *)malloc(buf),63);
 syscall_halt();
 return 0;
}
