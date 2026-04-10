#include "drivers/screen.h"
#include "kernel/idt.h"

void my_function() {

}

void main() {
    screen_init(VGA_WHITE, VGA_BLACK);
    terminal_writestring("HELLO 1\n");
    terminal_writestring("HELLO 2\n");

    idt_init();
    __asm__ volatile("sti");

    while(1) {
        __asm__ volatile("hlt");
    }
}