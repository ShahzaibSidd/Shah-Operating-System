#include "helper/string.h"

void* memset(void* s, int c, size_t n) {
    unsigned char* p = (unsigned char*)s;
    while (n) {
        *p++ = (unsigned char)c;
        n--;
    }
    return s;
}

void* memcpy(void* dest, const void* src, size_t n) {
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;
    while (n--) {
        *d++ = *s++;
    }
    return dest;
}

size_t strlen(char* data) {
    size_t count = 0;
    while (data[count]) {
        count++;
    }
    return count;
}

void stall(uint32_t time) {
    for (size_t i = 0; i < time; i++) {
        __asm__ volatile("nop");
    }
}