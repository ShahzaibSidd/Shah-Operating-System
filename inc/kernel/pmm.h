#include <stdint.h>

#define TOTAL_PAGES 1024;
#define PAGES_PER_BUCKET 32;

void pmm_mark_used(uint32_t page_addr);
void pmm_mark_free(uint32_t page_addr);
void* pmm_alloc_block();