#include <stdint.h>
#include <stddef.h>

#define MMAP_ADDR 0x8000
#define ENTRY_SIZE 0x18
#define NUM_ENTRIES 6

typedef struct {
    uint32_t base1;
    uint32_t base2;
    uint32_t length1;
    uint32_t length2;
    uint32_t type;
    uint32_t acpi;
} __attribute__((packed)) mmap_entry;

mmap_entry* get_mmap_entry(size_t n);

void print_mmap_entry(size_t n);