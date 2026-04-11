#include "drivers/screen.h"
#include "kernel/idt.h"

typedef struct {
    uint32_t base1;
    uint32_t base2;
    uint32_t length1;
    uint32_t length2;
    uint32_t type;
    uint32_t acpi;
} __attribute__((packed)) mmap_entry;

void my_function() {

}

void main() {
    screen_init(VGA_WHITE, VGA_BLACK);
    uint32_t test = 0xFEEDBEEF;
    terminal_writehex(test);
    terminal_newline();
    
    mmap_entry* mmap = (mmap_entry*)0x8000;
    terminal_writehex(mmap->base1);
    terminal_newline();
    terminal_writehex(mmap->base2);
    terminal_newline();
    terminal_writehex(mmap->length1);
    terminal_newline();
    terminal_writehex(mmap->length2);
    terminal_newline();
    terminal_writehex(mmap->type);
    terminal_newline();
    terminal_writehex(mmap->acpi);
    terminal_newline();

    
    idt_init();
    __asm__ volatile("sti");

    while(1) {
        __asm__ volatile("hlt");
    }
}