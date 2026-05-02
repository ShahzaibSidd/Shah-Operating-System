#include "drivers/screen.h"
#include "kernel/gdt.h"
#include "kernel/idt.h"
#include "kernel/mem_map.h"
#include "kernel/pmm.h"
#include "kernel/paging.h"
#include "kernel/heap.h"
#include "kernel/process.h"

void user_mode_function() {
    // Read CS to check privilege level
    uint16_t cs;
    __asm__ volatile("mov %%cs, %0" : "=r"(cs));

    // The bottom 2 bits of CS are the CPL (Current Privilege Level)
    if ((cs & 0x3) == 3) {
        // check if we're in Ring 3!
        // we don't have syscalls, write directly to video memory.
        // only works temporarily while i still have video memory is mapped with User permissions.
        volatile char* video_mem = (volatile char*)0xC00B8000;
        *video_mem = '3';
        *(video_mem + 1) = 0x0F; // White on Black
    }

    while(1) {
        __asm__ volatile("hlt"); 
    }
}

extern process_t* process_head;

void main() {
    screen_init(VGA_WHITE, VGA_BLACK);
    //draw_init_anim()

    gdt_init();
    idt_init();
    pmm_init();
    paging_init();
    paging_identity_del();
    
    heap_init();
    terminal_newline();

    terminal_writestring("Kernel initialized. Creating user process...\n");

    create_process((uint32_t)user_mode_function);

    terminal_writestring("Starting user process...\n");
    run_process(process_head);

    __asm__ volatile("sti");

    while(1) {
        __asm__ volatile("hlt");
    }
}