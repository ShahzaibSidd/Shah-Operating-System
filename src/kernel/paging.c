#include "kernel/paging.h"
#include "kernel/pmm.h"

pd_entry_t* kernel_directory;
pt_entry_t* first_table;

void paging_init() {
    kernel_directory = (pd_entry_t*) pmm_alloc_page();
    memset(kernel_directory, 0, BYTES_PER_PAGE);
    
    first_table = (pt_entry_t*) pmm_alloc_page();
    memset(first_table, 0, BYTES_PER_PAGE);

    for (size_t i = 0; i < 1024; i++) {
        first_table[i].frame = i;
        first_table[i].present = 1;
        first_table[i].rw = 1;
    }

    // Identity map (0x00000000 - 0x003FFFFF)
    kernel_directory[0].table = ((uint32_t)first_table >> 12);
    kernel_directory[0].present = 1;
    kernel_directory[0].rw = 1;

    // Higher-half map (0xC0000000 - 0xC03FFFFF)
    // 0xC0000000 >> 22 = 768
    kernel_directory[768].table = ((uint32_t)first_table >> 12);
    kernel_directory[768].present = 1;
    kernel_directory[768].rw = 1;

    load_page_directory((uint32_t)kernel_directory);
    enable_paging();
}