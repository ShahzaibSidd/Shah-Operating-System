#include "kernel/pmm.h"
#include "drivers/screen.h"

extern void kernel_end;

static uint32_t page_bitmap[BITMAP_SIZE];

void pmm_init() {
    for (size_t i = 0; i < BITMAP_SIZE; i++) {
        page_bitmap[i] = 0;
    }

    uint32_t kernel_end_addr = (uint32_t)&kernel_end;
    uint32_t kernel_pages =  (kernel_end_addr - RAM_START) / BYTES_PER_PAGE;
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
    return (page_bitmap[bucket] >> ind) & 1;
}

void pmm_mark_used(uint32_t page_addr) {
    uint32_t page_num = (page_addr - RAM_START) / BYTES_PER_PAGE;
    set_bitmap_ind(page_num);
}

void pmm_mark_free(uint32_t page_addr) {
    uint32_t page_num = (page_addr - RAM_START) / BYTES_PER_PAGE;
    free_bitmap_ind(page_num);
}

void* pmm_alloc_page() {
    for (size_t i = 0; i < BITMAP_SIZE; i++) {
        if (is_bitmap_ind_free(i)) {
            set_bitmap_ind(i);
            
            return (void*) (RAM_START + (BYTES_PER_PAGE * i));
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