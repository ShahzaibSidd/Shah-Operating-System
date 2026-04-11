#include "drivers/screen.h"
#include "kernel/idt.h"
#include "kernel/mem_map.h"
#include "kernel/pmm.h"

void main() {
    screen_init(VGA_WHITE, VGA_BLACK);   
    print_mmap_entry(0);
    print_mmap_entry(1);
    print_mmap_entry(2);
    print_mmap_entry(3);
    print_mmap_entry(4);
    print_mmap_entry(5);

    terminal_newline();

    bitmap_init();

    idt_init();
    __asm__ volatile("sti");

    while(1) {
        __asm__ volatile("hlt");
    }
}