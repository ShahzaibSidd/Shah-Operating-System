#include <stdint.h>
#include <stddef.h>

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