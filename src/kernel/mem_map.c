#include "kernel/mem_map.h"
#include "drivers/screen.h"

mmap_entry* get_mmap_entry(size_t n) {
    return (mmap_entry*) (MMAP_ADDR + (ENTRY_SIZE * n));
}



void print_mmap_entry(size_t n) {
    mmap_entry* curr_entry = get_mmap_entry(n);

    terminal_writestring("MEMORY SPACE: ");
    terminal_writehex(curr_entry->base1, true);
    terminal_writestring(" - ");

    terminal_writehex(curr_entry->base1 + curr_entry->length1 - 1, true);

    terminal_writestring(" SIZE: ");
    terminal_writehex(curr_entry->length1, true);

    terminal_writestring(" TYPE: ");
    if (curr_entry->type == 1) {
        terminal_writestring("FREE");
    } else {
        terminal_writestring("USED");
    }
    terminal_writechar('\n');
}