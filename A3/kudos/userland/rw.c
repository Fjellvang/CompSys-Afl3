/*
 * Halt the system from userland.
 */

#include "lib.h"


size_t buf = 64;

int main(void) {
 char buf3[] = "Du kan nu skrive til konsollen:\n";
 syscall_write(1, buf3, strlen(buf3));
 char buf5[20];
 buf5[19] = '\0';
 syscall_read(0, buf5, 20);
 syscall_write(1, buf5, 20);
 printf("\n");
 return 0;
}
