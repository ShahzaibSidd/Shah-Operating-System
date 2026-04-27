#include "kernel/process.h"
#include "kernel/paging.h"
#include "kernel/heap.h"
#include "kernel/pmm.h"
#include "kernel/vmm.h"
#include "helper/string.h"

process_t* process_head = NULL;
uint32_t curr_pid = 0;

void create_process(uint32_t entry_point) {
    // create process object
    process_t* new_proc = (process_t*)kmalloc(sizeof(process_t));
    memset(new_proc, 0, sizeof(process_t));
    new_proc->pid = curr_pid++;

    // alloc memory for new pd
    uint32_t pd_phys = (uint32_t)pmm_alloc_page();
    new_proc->page_directory = (pd_entry_t*)pd_phys;

    // make temp virt pd address to initialize
    void* temp_pd_virt = (void*)0xE0000000;
    map_page(get_page_directory(), temp_pd_virt, pd_phys, PAGE_PRESENT | PAGE_RW, PAGE_PRESENT | PAGE_RW);
    page_dir_init((pd_entry_t*)temp_pd_virt, pd_phys);
    unmap_page(get_page_directory(), temp_pd_virt);

    // Allocate Kernel Stack
    new_proc->kernel_stack = (uint32_t)kmalloc(BYTES_PER_PAGE) + BYTES_PER_PAGE;

    // Allocate and map User Stack
    uint32_t user_stack_phys = (uint32_t)pmm_alloc_page();
    uint32_t user_stack_virt = 0xBFFFF000; // Top of user space
    new_proc->user_stack = user_stack_virt + BYTES_PER_PAGE;

    // Map the user stack into the new process's page directory
    map_page(pd_phys, (void*)user_stack_virt, user_stack_phys, 
             PAGE_PRESENT | PAGE_RW | PAGE_USER, 
             PAGE_PRESENT | PAGE_RW | PAGE_USER);

    // Initialize context (registers)
    new_proc->context.eip = entry_point;
    new_proc->context.esp = new_proc->user_stack;
    new_proc->context.ebp = new_proc->user_stack;
    
    // segment selectors from GDT, User Code is 0x1B, User Data is 0x23
    new_proc->context.cs = 0x1B;
    new_proc->context.ds = 0x23;
    new_proc->context.ss = 0x23;
    new_proc->context.es = 0x23;
    new_proc->context.fs = 0x23;
    new_proc->context.gs = 0x23;
    
    // eflags bit 9 is Interrupt Enable Flag
    new_proc->context.eflags = 0x202; 

    // add to Process List
    if (!process_head) {
        process_head = new_proc;
    } else {
        process_t* curr = process_head;
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = new_proc;
    }
}
