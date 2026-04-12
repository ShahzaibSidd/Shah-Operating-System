#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include <stddef.h>
#include "helper/string.h"

typedef struct {
    uint32_t present    : 1;
    uint32_t rw         : 1;
    uint32_t user       : 1;
    uint32_t pwt        : 1;   // Page-level write-through
    uint32_t pcd        : 1;   // Page-level cache disable
    uint32_t accessed   : 1;
    uint32_t ignored    : 1;
    uint32_t os_size    : 1;   // 0 = 4KB page, 1 = 4MB page
    uint32_t unused     : 4;
    uint32_t table      : 20;  // Physical address of the Page Table
} __attribute__((packed)) pd_entry_t;

typedef struct {
    uint32_t present    : 1;   // Page is in RAM
    uint32_t rw         : 1;   // 0 = Read-only, 1 = Read/Write
    uint32_t user       : 1;   // 0 = Supervisor only, 1 = User can access
    uint32_t accessed   : 1;   // Set by CPU when read
    uint32_t dirty      : 1;   // Set by CPU when written to
    uint32_t unused     : 7;   // Reserved/Available for OS use
    uint32_t frame      : 20;  // The upper 20 bits of the physical address
} __attribute__((packed)) pt_entry_t;

// load physical papge directory address to cr3
static inline void load_page_directory(uint32_t pd_addr) {
    __asm__ volatile("mov %0, %%cr3" : : "r"(pd_addr) : "memory");
}

static inline void enable_paging() {
    uint32_t cr0;
    __asm__ volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ volatile("mov %0, %%cr0" : : "r"(cr0) : "memory");
}

void paging_init();

#endif
