#include "drivers/screen.h"
#include "kernel/gdt.h"
#include "kernel/idt.h"
#include "kernel/mem_map.h"
#include "kernel/pmm.h"
#include "kernel/paging.h"
#include "kernel/heap.h"

void main() {
    screen_init(VGA_WHITE, VGA_BLACK);
    
    gdt_init();
    idt_init();
    pmm_init();
    paging_init();
    paging_identity_del();

    heap_init();

    terminal_writestring("testing\n");
    print_kernel_end();
    terminal_newline();

    uint16_t* new_val = kmalloc(2);
    terminal_writehex((uint32_t)&new_val, true);

    __asm__ volatile("sti");

    while(1) {
        __asm__ volatile("hlt");
    }
}