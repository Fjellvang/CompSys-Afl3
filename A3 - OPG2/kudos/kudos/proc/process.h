// Kernel support for userland processes.

#ifndef KUDOS_PROC_PROCESS_H
#define KUDOS_PROC_PROCESS_H

#include "lib/types.h"
#include "vm/memory.h"
#include <cswitch.h>
//Panisk tilføjelse af include cases
#include <pagetable.h>
#include <kernel/types.h>

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
  context_t user_context;
  virtaddr_t entry_point;

  virtaddr_t stack_top;

  pid_t pidns;
 
} pcb_t;


void process_start(const char *path);
void process_write(int fd, void *buf, int nbytes);
void process_read(int fd, void *buf, int nbytes);

//initialize process table
//Should be called during boot
void process_init(void);

//load and run the executavle as a new process in a new thread.
//Arguments: path to the executable and 
//           flags specifying the desired level of sharing
//Returns the process ID of the new process
pid_t process_spawn(char const *path, int flags);

//return PID of current process
pid_t process_get_current_process(void);

//return PCD of current process.
pcb_t *process_get_current_process_entry(void);

#endif // KUDOS_PROC_PROCESS_H
