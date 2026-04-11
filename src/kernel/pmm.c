#include "kernel/pmm.h"

static uint32_t page_bitmap[FREE_RAM / BYTES_PER_PAGE];

void bitmap_init() {
    size_t bitmap_size = FREE_RAM / BYTES_PER_PAGE;
    for (size_t i = 0; i < bitmap_size; i++) {
        page_bitmap[i] = 0;
    }
}

void pmm_mark_used(uint32_t page_addr) {
    uint32_t ind = page_addr / 4096;
}

void pmm_mark_free(uint32_t page_addr) {

}

void* pmm_alloc_block() {

}