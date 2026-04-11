#include <stdint.h>
#include <stddef.h>

#define FREE_RAM 0x7EE0000
#define RAM_START 0x100000
#define BYTES_PER_PAGE 4096
#define PAGES_PER_BUCKET 32

void bitmap_init();

size_t set_bitmap_ind(uint32_t page_num);

size_t free_bitmap_ind(uint32_t page_num);

void pmm_mark_used(uint32_t page_addr);
void pmm_mark_free(uint32_t page_addr);
void* pmm_alloc_block();

void print_bitmap();