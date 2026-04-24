#include <stddef.h>
#include <stdint.h>

size_t strlen(char* data);
void stall(uint32_t time);

void* memset(void* s, int c, size_t n);
void* memcpy(void* dest, const void* src, size_t n);
