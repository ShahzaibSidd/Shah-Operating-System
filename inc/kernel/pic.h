#include <stdint.h>

#include "kernel/low_level.h"

#define PIC1 0x20
#define PIC2 0xA0

void pic_remap() {
    // initialization command word 1 (ICW1)
    port_byte_out(0x20, 0x11);
    port_byte_out(0xA0, 0x11);
}
