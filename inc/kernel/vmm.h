#include <stdint.h>

#define PAGE_PRESENT  0x01   // Bit 0
#define PAGE_RW       0x02   // Bit 1
#define PAGE_USER     0x04   // Bit 2
#define PAGE_PWT      0x08   // Bit 3 (Write-through caching)
#define PAGE_PCD      0x10   // Bit 4 (Cache disable)
#define PAGE_ACCESSED 0x20   // Bit 5 (Set by CPU when read)
#define PAGE_DIRTY    0x40   // Bit 6 (Set by CPU when written to - PTE only)

void map_page(void* virt_addr, uint32_t phys_addr, uint32_t pd_flags, uint32_t pt_flags);
void unmap_page(void* virt_addr);