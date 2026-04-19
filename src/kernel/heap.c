#include "kernel/heap.h"
#include "kernel/pmm.h"
#include "kernel/vmm.h"
#include "drivers/screen.h"

heap_node_t* heap_head = NULL;
uint32_t heap_curr_top = HEAP_START;
uint32_t heap_struct_size = sizeof(heap_node_t);

void heap_init() {
    // allocate first page for heap
    uint32_t phys_page_addr = (uint32_t)pmm_alloc_page();
    uint32_t flags = PAGE_PRESENT | PAGE_RW;
    
    // map the page to virtual address
    map_page((void*)heap_curr_top, phys_page_addr, flags, flags);
    
    // set up head of linked list heap
    heap_head = (heap_node_t*)HEAP_START;
    heap_head->size = BYTES_PER_PAGE - heap_struct_size;
    heap_head->is_free = 1;
    heap_head->next = NULL;
    
    heap_curr_top += BYTES_PER_PAGE;

    terminal_writestring("HEAP HEAD: ");
    terminal_writehex((uint32_t)heap_head, true);
}

void* kmalloc(uint32_t malloc_size) {
    // allign size to largest 4 byte edge
    malloc_size = (malloc_size + 3) & ~(3);
    uint32_t min_split_size = malloc_size + heap_struct_size + 4;

    heap_node_t* last = NULL;
    for (heap_node_t* curr = heap_head; curr != NULL; curr = curr->next) {
        if (curr->is_free && (malloc_size <= curr->size)) {
            if (min_split_size <= curr->size) {                
                heap_node_t* new_block = (heap_node_t*)((uint8_t*)curr + heap_struct_size + malloc_size);

                new_block->is_free = 1;
                new_block->size = curr->size - malloc_size - heap_struct_size;
                new_block->next = curr->next;
    
                curr->size = malloc_size;
                curr->next = new_block;
            }

            curr->is_free = 0;
            return (void*)((uint8_t*)curr + heap_struct_size);
        }
        last = curr;
    }
    return expand_heap(last, malloc_size);
}

void* expand_heap(heap_node_t* last_node, uint32_t malloc_size) {
    uint32_t needed_size = malloc_size + heap_struct_size;
    uint32_t pages_to_map = (needed_size / BYTES_PER_PAGE) + 1;
    
    heap_node_t* new_block = (heap_node_t*)heap_curr_top;

    for (size_t i = 0; i < pages_to_map; i++) {
        uint32_t phys_page_addr = (uint32_t)pmm_alloc_page();
        uint32_t flags = PAGE_PRESENT | PAGE_RW;
        
        map_page((void*)heap_curr_top, phys_page_addr, flags, flags);

        heap_curr_top += BYTES_PER_PAGE;
    }
    
    new_block->is_free = 1;
    new_block->size = (pages_to_map * 4096) - heap_struct_size;
    new_block->next = NULL;
    
    last_node->next = new_block;

    uint32_t min_split_size = malloc_size + heap_struct_size + 4;
    if (malloc_size <= new_block->size) {
        if (min_split_size <= new_block->size) {                
            heap_node_t* newer_block = (heap_node_t*)((uint8_t*)new_block + heap_struct_size + malloc_size);
            
            newer_block->is_free = 1;
            newer_block->size = new_block->size - malloc_size - heap_struct_size;
            newer_block->next = new_block->next;
            
            new_block->size = malloc_size;
            new_block->next = newer_block;
        }
        
        new_block->is_free = 0;
        return (void*)((uint8_t*)new_block + heap_struct_size);
    }

}


void kfree(void* addr) {

}