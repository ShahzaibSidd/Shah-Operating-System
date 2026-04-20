#include <stddef.h>
#include <stdint.h>

static inline size_t strlen(char* data) {
    size_t count = 0;
    while (data[count]) {
        count++;
    }
    return count;
}

static inline void stall(uint32_t time) {
    for (size_t i = 0; i < time; i++) {
        __asm__ volatile("nop");
    }
}

void* memset(void* s, int c, size_t n);
void* memcpy(void* dest, const void* src, size_t n);
