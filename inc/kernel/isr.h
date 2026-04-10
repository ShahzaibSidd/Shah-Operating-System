#pragma once

#include <stdint.h>

#include "kernel/low_level.h"
#include "drivers/screen.h"

typedef struct {
    uint32_t ip;
    uint32_t cs;
    uint32_t flags;
    uint32_t sp;
    uint32_t ss;
} interrupt_frame;

__attribute__((interrupt)) void keyboard_controller(interrupt_frame* frame);