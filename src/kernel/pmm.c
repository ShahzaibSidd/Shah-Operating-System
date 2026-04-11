#include "kernel/pmm.h"
#include "drivers/screen.h"

extern void kernel_end;

static uint32_t page_bitmap[FREE_RAM / BYTES_PER_PAGE / PAGES_PER_BUCKET];

void bitmap_init() {
    size_t bitmap_size = FREE_RAM / BYTES_PER_PAGE / PAGES_PER_BUCKET;
    for (size_t i = 0; i < bitmap_size; i++) {
        page_bitmap[i] = 0;
    }

    uint32_t kernel_end_addr = (uint32_t)&kernel_end;
    uint32_t kernel_pages =  (kernel_end_addr - RAM_START) / BYTES_PER_PAGE;
    for (size_t i = 0; i < kernel_pages; i++) {
        set_bitmap_ind(i);
    }

    terminal_writehex(kernel_end_addr, true);
    terminal_newline();
    print_bitmap();
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

void pmm_mark_used(uint32_t page_addr) {
    uint32_t page_num = (page_addr - RAM_START) / BYTES_PER_PAGE;
    set_bitmap_ind(page_num);
}

void pmm_mark_free(uint32_t page_addr) {
    uint32_t page_num = (page_addr - RAM_START) / BYTES_PER_PAGE;
    free_bitmap_ind(page_num);
}

void* pmm_alloc_block() {

}

void print_bitmap() {
    size_t bitmap_size = FREE_RAM / BYTES_PER_PAGE / PAGES_PER_BUCKET;
    for (size_t i = 0; i < 10; i++) {
        terminal_writehex(page_bitmap[i], true);
        terminal_newline();
    }
}