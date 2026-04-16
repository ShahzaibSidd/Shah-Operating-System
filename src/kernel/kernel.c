#include "drivers/screen.h"
#include "kernel/gdt.h"
#include "kernel/idt.h"
#include "kernel/mem_map.h"
#include "kernel/pmm.h"
#include "kernel/paging.h"

void main() {
    screen_init(VGA_WHITE, VGA_BLACK);
    
    gdt_init();
    idt_init();
    pmm_init();
    paging_init();
    paging_identity_del();

    terminal_writestring("testing\n");

    __asm__ volatile("sti");

    while(1) {
        __asm__ volatile("hlt");
    }
}