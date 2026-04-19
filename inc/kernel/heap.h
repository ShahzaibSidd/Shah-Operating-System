#include <stdint.h>
#include <stddef.h>

#define HEAP_START 0xD0000000

typedef struct heap_node {
    uint32_t size;
    uint32_t is_free;
    struct heap_node* next;
} heap_node_t;

void heap_init();
void* kmalloc(uint32_t malloc_size);
void* expand_heap(heap_node_t* last_node, uint32_t malloc_size);
void kfree(void* addr);

