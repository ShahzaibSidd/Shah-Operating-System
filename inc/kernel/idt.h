#pragma once

#include <stdint.h>
#include <stddef.h>

#include "kernel/low_level.h"

#define PIC1 0x20
#define PIC2 0xA0

static inline void pic_remap() {
    // initialization command word 1 (ICW1)
    port_byte_out(0x20, 0x11);
    port_byte_out(0xA0, 0x11);

    port_byte_out(0x21, 0x20);
    port_byte_out(0xA1, 0x28);

    port_byte_out(0x21, 0x04);
    port_byte_out(0xA1, 0x02);

    port_byte_out(0x21, 0x01);
    port_byte_out(0xA1, 0x01);

    port_byte_out(0x21, 0xFF);
    port_byte_out(0xA1, 0xFF);
    return;
}

static inline void pic_enable(uint8_t irq) {
    irq -= 32;
    if (irq < 8) {
        uint8_t val = port_byte_in(0x21);
        val = val & ~(1 << irq);
        port_byte_out(0x21, val);
    } else {
        // Unmask IRQ 2 on master PIC to allow slave PIC interrupts
        uint8_t master_val = port_byte_in(0x21);
        master_val = master_val & ~(1 << 2);
        port_byte_out(0x21, master_val);

        irq -= 8;
        uint8_t val = port_byte_in(0xA1);
        val = val & ~(1 << irq);
        port_byte_out(0xA1, val);
    }
}

typedef struct {
    uint16_t base_low;
    uint16_t segment_selector;
    uint8_t reserved;
    uint8_t flags;
    uint16_t base_high; 
} __attribute__((packed)) idt_entry;

typedef struct {
    uint16_t size;
    uint32_t offset;
} __attribute__((packed)) idt_descriptor;

void set_idt_gate(size_t n, uint32_t handler);
void idt_init();