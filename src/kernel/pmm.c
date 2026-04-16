#include "kernel/pmm.h"
#include "drivers/screen.h"

extern void kernel_end;

static uint32_t page_bitmap[BITMAP_SIZE];

void pmm_init() {
    // 1. Clear bitmap (all 0 = all free)
    for (size_t i = 0; i < BITMAP_SIZE; i++) {
        page_bitmap[i] = 0;
    }

    // 2. Mark kernel memory as used
    extern void kernel_end;
    uint32_t kernel_end_phys = (uint32_t)&kernel_end - 0xC0000000;
    
    // Mark from RAM_START to kernel_end as used
    for (uint32_t addr = RAM_START; addr < kernel_end_phys; addr += BYTES_PER_PAGE) {
        pmm_mark_used(addr);
    }
}

size_t set_bitmap_ind(uint32_t page_num) {
    size_t bucket = page_num / PAGES_PER_BUCKET;
    size_t ind = page_num % PAGES_PER_BUCKET;
    page_bitmap[bucket] |= (uint32_t) (1 << ind);
}

size_t free_bitmap_ind(uint32_t page_num) {
    size_t bucket = page_num / PAGES_PER_BUCKET;
    size_t ind = page_num % PAGES_PER_BUCKET;
    page_bitmap[bucket] &= (uint32_t) ~(1 << ind);
}

bool is_bitmap_ind_free(uint32_t page_num) {
    size_t bucket = page_num / PAGES_PER_BUCKET;
    size_t ind = page_num % PAGES_PER_BUCKET;
    return !((page_bitmap[bucket] >> ind) & 1);
}

void pmm_mark_used(uint32_t page_addr) {
    uint32_t page_num = page_addr / BYTES_PER_PAGE;
    set_bitmap_ind(page_num);
}

void pmm_mark_free(uint32_t page_addr) {
    uint32_t page_num = page_addr / BYTES_PER_PAGE;
    free_bitmap_ind(page_num);
}

void* pmm_alloc_page() {
    for (size_t i = RAM_START / BYTES_PER_PAGE; i < BITMAP_SIZE * PAGES_PER_BUCKET; i++) {
        if (is_bitmap_ind_free(i)) {
            set_bitmap_ind(i);
            
            return (void*) (BYTES_PER_PAGE * i);
        }
    }
    return NULL;
}

void print_bitmap() {
    for (size_t i = 0; i < 10; i++) {
        terminal_writehex(page_bitmap[i], true);
        terminal_newline();
    }
}