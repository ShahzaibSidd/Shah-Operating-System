#include "kernel/gdt.h"
#include "helper/string.h"

gdt_entry gdt[6];
gdt_ptr gp;

tss_entry tss;

void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;

    gdt[num].granularity |= gran & 0xF0;
    gdt[num].access = access;
}

void gdt_init() {
    gp.limit = (sizeof(gdt_entry) * 6) - 1;
    gp.base = (uint32_t)&gdt;

    gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Kernel Code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Kernel Data segment
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User Code segment
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User Data segment

    tss_init();
    uint32_t tss_base = (uint32_t)&tss;
    uint32_t tss_limit = sizeof(tss_entry) - 1;
    // TSS access byte: 0x89 (Present, Executable, Accessible from Ring 0)
    gdt_set_gate(5, tss_base, tss_limit, 0x89, 0x00);

    __asm__ volatile("lgdt %0" : : "m"(gp));

    // TSS is at index 5, so selector is 5 * 8 = 0x28
    uint16_t tss_selector = 0x28;
    __asm__ volatile("ltr %0" : : "r"(tss_selector));

    // Reload segment registers
    __asm__ volatile(
        "mov $0x10, %ax\n\t"
        "mov %ax, %ds\n\t"
        "mov %ax, %es\n\t"
        "mov %ax, %ss\n\t"
        "mov %ax, %fs\n\t"
        "mov %ax, %gs\n\t"
        "ljmp $0x08, $1f\n\t"
        "1:\n\t"
    );
}


void tss_update_stack_pointer(uint32_t addr) {
    tss.esp0 = addr;
}

void tss_init() {
    // Zero out the TSS to avoid garbage in reserved fields
    memset(&tss, 0, sizeof(tss_entry));

    uint32_t current_stack;
    __asm__ volatile("mov %%esp, %0" : "=r"(current_stack));
    tss_update_stack_pointer(current_stack);

    // kernel data segment is at index 2 in gdt, 0x08 * 2 = 0x10
    tss.ss0 = 0x10;

    // Set iomap_base to sizeof(tss) to indicate no IO map is present
    tss.iomap_base = sizeof(tss_entry);
}