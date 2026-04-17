#include "kernel/vmm.h"

#include "kernel/paging.h"
#include "kernel/pmm.h"
#include "helper/string.h"

void map_page(void* virt_addr, uint32_t phys_addr, uint32_t pd_flags, uint32_t pt_flags) {
    uint32_t va = (uint32_t)virt_addr;
    
    // top 10 bits
    uint32_t pd_ind = va >> 22;

    // middle 10 bits
    uint32_t pt_ind = (va >> 12) & 0x3FF;

    uint32_t* page_dir = (uint32_t*)0xFFFFF000;
    uint32_t* page_table = (uint32_t*)(0xFFC00000 + (pd_ind * 0x1000));

    if (!(page_dir[pd_ind] & 0x01)) {
        pt_entry_t* new_page_table = pmm_alloc_page();
        new_page_table = (phys_addr & 0xFFFFF000) | pd_flags | 0x01;
        page_dir[pd_ind] = new_page_table;
        memset(page_table, 0, 4096);
    }

    page_table[pt_ind] = (phys_addr & 0xFFFFF000) | pt_flags | 0x01;
    __asm__ volatile("invlpg (%0)" : : "r"(virt_addr) : "memory");
}

void unmap_page(void* virt_addr) {
    uint32_t va = (uint32_t)virt_addr;
    
    // top 10 bits
    uint32_t pd_ind = va >> 22;

    // middle 10 bits
    uint32_t pt_ind = (va >> 12) & 0x3FF;

    uint32_t* page_dir = (uint32_t*)0xFFFFF000;
    uint32_t* page_table = (uint32_t*)(0xFFC00000 + (pd_ind * 0x1000));

    if (!(page_dir[pd_ind] & 0x01)) {
        return;
    }

    page_table[pt_ind] = 0;
    __asm__ volatile("invlpg (%0)" : : "r"(virt_addr) : "memory");
    
}