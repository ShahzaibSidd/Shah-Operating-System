#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define FREE_RAM 0x7EE0000UL
#define RAM_START 0x100000UL
#define BYTES_PER_PAGE 4096UL
#define PAGES_PER_BUCKET 32
#define BITMAP_SIZE (FREE_RAM / BYTES_PER_PAGE / PAGES_PER_BUCKET)

void pmm_init();

size_t set_bitmap_ind(uint32_t page_num);
size_t free_bitmap_ind(uint32_t page_num);
bool is_bitmap_ind_free(uint32_t page_num);

void pmm_mark_used(uint32_t page_addr);
void pmm_mark_free(uint32_t page_addr);
void* pmm_alloc_page();

void print_bitmap();