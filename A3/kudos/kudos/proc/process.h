/// Kernel support for userland processes.

#ifndef KUDOS_PROC_PROCESS_H
#define KUDOS_PROC_PROCESS_H

#include "lib/types.h"
#include "vm/memory.h"
#include <cswitch.h>
//#include <pagetable.h>
//#include <kernel/types.h>
//FDT

#define PROCESS_PTABLE_FULL  (-1)
#define PROCESS_ILLEGAL_JOIN (-2)

#define PROCESS_MAX_FILELENGTH (256)
#define PROCESS_MAX_PROCESSES  (128)
#define PROCESS_MAX_FILES      (10)

typedef int pid_t;

typedef enum {
  PROCESS_FREE,
  PROCESS_WORKING
} process_state_t;

typedef struct {
  // TODO: Define a pid namespace here.
  pid_t pid;
  process_state_t state;
  //context_t user_context;
  //virtaddr_t entry_point;
  //virtaddr_t stack_top;

  fdt_t fd;
} pcb_t;


void process_start(const char *path);
void process_write(int fd, void *buf, int nbytes);
void process_read(int fd, void *buf, int nbytes);

#endif // KUDOS_PROC_PROCESS_H
