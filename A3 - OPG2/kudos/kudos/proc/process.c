/// Kernel support for userland processes.

#include <arch.h>
#include "proc/process.h"
#include "proc/elf.h"
#include "kernel/thread.h"
#include "kernel/assert.h"
#include "kernel/interrupt.h"
#include "kernel/config.h"
#include "fs/vfs.h"
#include "kernel/sleepq.h"
#include "vm/memory.h"
//TILFØJET
#include "drivers/gcd.h"

extern void process_set_pagetable(pagetable_t*);

pcb_t process_table[PROCESS_MAX_PROCESSES];

/* Return non-zero on error. */
int setup_new_process(TID_t thread,
                      const char *path,
                      virtaddr_t *entry_point, virtaddr_t *stack_top)
{
  pagetable_t *pagetable;
  elf_info_t elf;
  openfile_t file;
  uintptr_t phys_page;
  virtaddr_t virt_page;
  int i, res;
  thread_table_t *thread_entry = thread_get_thread_entry(thread);

  file = vfs_open((char *)path);

  /* Make sure the file existed and was a valid ELF file */
  if (file < 0) {
    return -1;
  }

  res = elf_parse_header(&elf, file);
  if (res < 0) {
    return -1;
  }

  /* Trivial and naive sanity check for entry point: */
  if (elf.entry_point <= VMM_KERNEL_SPACE) {
    return -1;
  }

  *entry_point = elf.entry_point;

  pagetable = vm_create_pagetable(thread);
  process_set_pagetable(pagetable);

  thread_entry->pagetable = pagetable;

  /* Allocate and map stack */
  for(i = 0; i < CONFIG_USERLAND_STACK_SIZE; i++) {
    phys_page = physmem_allocblock();
    KERNEL_ASSERT(phys_page != 0);
    virt_page = (USERLAND_STACK_TOP & PAGE_SIZE_MASK) - i*PAGE_SIZE;
    vm_map(pagetable, phys_page,
           virt_page, PAGE_USER | PAGE_WRITE);
    /* Zero the page */
    memoryset((void*)virt_page, 0, PAGE_SIZE);
  }

  /* Allocate and map pages for the ELF segments. We assume that
     the segments begin at a page boundary. (The linker script
     in the userland directory helps users get this right.) */
  for(i = 0; i < (int)elf.ro_pages; i++) {
    int left_to_read = elf.ro_size - i*PAGE_SIZE;
    phys_page = physmem_allocblock();
    KERNEL_ASSERT(phys_page != 0);
    virt_page = elf.ro_vaddr + i*PAGE_SIZE;
    vm_map(pagetable, phys_page,
           virt_page, PAGE_USER | PAGE_WRITE);
    /* Zero the page */
    memoryset((void*)virt_page, 0, PAGE_SIZE);
    /* Fill the page from ro segment */
    if (left_to_read > 0) {
      KERNEL_ASSERT(vfs_seek(file, elf.ro_location + i*PAGE_SIZE) == VFS_OK);
      KERNEL_ASSERT(vfs_read(file, virt_page,
                             MIN(PAGE_SIZE, left_to_read))
                    == (int) MIN(PAGE_SIZE, left_to_read));
    }
    //Make the page read only
    vm_map(pagetable, phys_page,
            virt_page, PAGE_USER);
  }

  for(i = 0; i < (int)elf.rw_pages; i++) {
    int left_to_read = elf.rw_size - i*PAGE_SIZE;
    phys_page = physmem_allocblock();
    KERNEL_ASSERT(phys_page != 0);
    virt_page = elf.rw_vaddr + i*PAGE_SIZE;
    vm_map(pagetable, phys_page,
           virt_page, PAGE_USER | PAGE_WRITE);
    /* Zero the page */
    memoryset((void*)virt_page, 0, PAGE_SIZE);
    /* Fill the page from rw segment */
    if (left_to_read > 0) {
      KERNEL_ASSERT(vfs_seek(file, elf.rw_location + i*PAGE_SIZE) == VFS_OK);
      KERNEL_ASSERT(vfs_read(file, (void*)virt_page,
                             MIN(PAGE_SIZE, left_to_read))
                    == (int) MIN(PAGE_SIZE, left_to_read));
    }
  }

  /* Done with the file. */
  vfs_close(file);

  *stack_top = USERLAND_STACK_TOP;

  return 0;
}

void init_user_context(uint32_t i){

  pcb_t *pcb = &process_table[i];

  memoryset(pcb->user_context, 0, sizeof(pcb->user_context));

  _context_set_ip(pcb->user_context, pcb->entry_point);
  _context_set_sp(pcb->user_context, pcb->stack_top);

  thread_goto_userland(user_context);
}

pid_t process_spawn(char const *path, int flags){

  if(flags != 0 ) {
    //fail for all non-zero values
    kprintf("FAILURE");
    return;
  } 

  TID_t new_thread;
  virtaddr_t entry_point;
  int ret,i;
//MÅSKE

  pcb_t *pcb;
  
  for(i = 0; i < PROCESS_MAX_PROCESSES; i++){

    // måske
    pid_t p = i % PROCESS_MAX_PROCESSES;
    
    if(process_table[i].state == PROCESS_FREE){
      pcb = &process_table[i];
      pcb->pid = p;
      new_thread = thread_create(init_user_context, (uint32_t)i);
      ret = setup_new_process(new_thread, path,
                              &pcb->entry_point, &pcb->stack_top);
      thread_run(new_thread);
      thread_get_thread_entry(new_thread)->pid = pcb->pid;
      //process_table[i].pidns = pcb->pid;
      break;
    }  

  }
  return process_table[i].pid;
}

void process_init(){
  for(int i = 0; i < PROCESS_MAX_PROCESSES; i++){
    process_table[i].state = PROCESS_FREE; 
  }  

}
//MÅSKKKKKKKKKKKKKKKEEE
void process_new(uint32_t arg){
  

}



//void process_start(const char *path)
//{
  //TID_t my_thread;
  //virtaddr_t entry_point;
  //int ret;
  //context_t user_context;
  //virtaddr_t stack_top;

  //ret = setup_new_process(my_thread, path,
                          //&entry_point, &stack_top);

  //if (ret != 0) {
    //return; /* Something went wrong. */
  //}

  /* Initialize the user context. (Status register is handled by
     thread_goto_userland) */
  //memoryset(&user_context, 0, sizeof(user_context));

  //_context_set_ip(&user_context, entry_point);
  //_context_set_sp(&user_context, stack_top);

  //thread_goto_userland(&user_context);
//}
// TILFØJETTT------------------------------------------------------------------------------------------------------------------------------------------
//void process_new(uintptr_t arg){
  //for(int i = 0; i < PROCESS_MAX_PROCESSES; i++){
    //pid_t t = i % PROCESS_MAX_PROCESSES;  
    
    //if(process_table[i].state == PROCESS_FREE){
        

    //}
  //}
//}
// TUKFÆJETTTS--------------------------------------------------------------------------------------------------------------------------------
void process_write(int fd, void *buf, int nbytes){
  device_t *dev;
  gcd_t *gcd;
   
  dev = device_get(TYPECODE_TTY, 0);
  KERNEL_ASSERT(dev != NULL);

  gcd = (gcd_t *)dev->generic_device;
  KERNEL_ASSERT(gcd != NULL);
  gcd->write(gcd, buf, nbytes);
}

void process_read(int fd, void *buf, int nbytes){
  
  device_t *dev;
  gcd_t *gcd;
  dev = device_get(TYPECODE_TTY, 0);
  KERNEL_ASSERT(dev != NULL);

  gcd = (gcd_t *)dev->generic_device;
  KERNEL_ASSERT(gcd != NULL);
  
  int len = gcd->read(gcd, buf, nbytes); 
  KERNEL_ASSERT(len>= 0);
}
