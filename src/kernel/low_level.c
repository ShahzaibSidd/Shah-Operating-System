#include "kernel/low_level.h"
#include "drivers/screen.h"

extern void kernel_end;

unsigned char port_byte_in(unsigned short port) {
    unsigned char result;
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void port_byte_out(unsigned short port, unsigned short data) {
    __asm__("out %%al, %%dx" : :"a" (data), "d" (port));
    return;
}

unsigned short port_word_in(unsigned short port) {
    unsigned short result;
    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

void port_word_out(unsigned short port, unsigned short data) {
    __asm__("out %%ax, %%dx" : :"a" (data), "d" (port));
    return;
}

void print_kernel_end() {
    terminal_writehex((uint32_t)&kernel_end, true);
    terminal_newline();
}