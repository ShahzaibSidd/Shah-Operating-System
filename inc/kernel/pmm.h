#include <stdint.h>
#include <stddef.h>

#define FREE_RAM 0x7EE0000
#define BYTES_PER_PAGE 4096
#define PAGES_PER_BUCKET 32

void bitmap_init();
void pmm_mark_used(uint32_t page_addr);
void pmm_mark_free(uint32_t page_addr);
void* pmm_alloc_block();