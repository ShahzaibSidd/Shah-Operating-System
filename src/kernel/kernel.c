#include "drivers/screen.h"
#include "kernel/idt.h"
#include "kernel/mem_map.h"
#include "kernel/pmm.h"
#include "kernel/paging.h"

void main() {
    screen_init(VGA_WHITE, VGA_BLACK);
    
    pmm_init();
    paging_init();

    print_bitmap();

    idt_init();
    __asm__ volatile("sti");

    while(1) {
        __asm__ volatile("hlt");
    }
}