#include <stdint.h>

void map_page(void* virt_addr, uint32_t phys_addr, uint32_t pd_flags, uint32_t pt_flags);
void unmap_page(void* virt_addr);