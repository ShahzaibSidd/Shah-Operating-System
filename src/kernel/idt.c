#include "kernel/idt.h"
#include "kernel/isr.h"

static idt_entry idt[256];
static idt_descriptor curr_descriptor;


void set_idt_gate(size_t n, uint32_t handler) {
    idt[n].base_low = (uint16_t) (handler & 0xFFFF);
    
    // we want code segment, 0x08 is code segment offset
    idt[n].segment_selector = 0x08;
    
    // has to be 0
    idt[n].reserved = 0;
    
    // 1 00 0 1110 = 0x8E
    idt[n].flags = 0x8E;

    idt[n].base_high = (uint16_t) (handler >> 16) & 0xFFFF;
    
    pic_enable(n);
}

void idt_init() {
    // re-map pic irqs 0-15 to 32-40
    pic_remap();


    set_idt_gate(0x21, (uint32_t)keyboard_controller);

    // build idt descriptor
    curr_descriptor.size = (sizeof(idt_entry) * 256) - 1;
    curr_descriptor.offset = (uint32_t) &idt;

    //lidt command to set
    __asm__ volatile("lidt %0" : : "m"(curr_descriptor));
}
