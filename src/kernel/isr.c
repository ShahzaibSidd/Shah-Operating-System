#include "kernel/isr.h"
#include "drivers/keyboard.h"

__attribute__((interrupt)) void keyboard_controller(interrupt_frame* frame) {
    uint8_t scancode = port_byte_in(0x60);

    handle_key_press(scancode);

    // send EOI to PIC
    port_byte_out(0x20, 0x20);
}