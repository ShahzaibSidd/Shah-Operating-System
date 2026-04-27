#include <stdint.h>
#include <stddef.h>
#include "kernel/paging.h"

typedef struct {
    uint32_t eax, ebx, ecx, edx;        // General purpose
    uint32_t esi, edi, esp, ebp;        // Stack and index
    uint32_t eip;                       // Instruction pointer
    uint32_t eflags;                    // CPU flags
    uint32_t cs, ss, ds, es, fs, gs;    // Segment selectors
} __attribute__((packed)) registers_t;


typedef struct process {
    uint32_t pid;                   // Unique Process ID
    registers_t context;            // The saved CPU state
                                    
    // Stacks
    uint32_t kernel_stack;          // Pointer to the UNIQUE kernel stack
    uint32_t user_stack;            // Pointer to the user-mode stack

    // Memory Isolation
    pd_entry_t* page_directory;     // Physical address of this process's page directory 
                                    
    struct process* next;           // For the process linked list
} __attribute__((packed)) process_t;

void create_process(uint32_t entry_point);

