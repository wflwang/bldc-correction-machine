/*
 * utils_mem.c
 * Simple memory utilities for embedded systems
 */

#include <stdint.h>
#include <stddef.h>

/**
 * Fill memory with a constant byte
 * 
 * @param dest Destination memory
 * @param c Constant byte
 * @param n Number of bytes to fill
 * @return Destination memory
 */
void *memset(void *dest, int c, size_t n) {
    uint8_t *d = (uint8_t *)dest;
    uint8_t value = (uint8_t)c;
    
    while (n--) {
        *d++ = value;
    }
    
    return dest;
}
