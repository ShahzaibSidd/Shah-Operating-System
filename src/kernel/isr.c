#include "kernel/isr.h"

__attribute__((interrupt)) void keyboard_controller(interrupt_frame* frame) {
    uint8_t scancode = port_byte_in(0x60);

    // TODO: actually handling the keyboard key-press and doing smth
    terminal_writestring("KEYBOARD INTERRUPT\n");

    // send EOI to PIC
    port_byte_out(0x20, 0x20);
}