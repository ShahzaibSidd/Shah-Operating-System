#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "kernel/low_level.h"

#define WIDTH 80
#define HEIGHT 25
#define VGA_MEM 0xC00B8000

#define IDX_PORT  0x3D4
#define DATA_PORT 0x3D5


typedef enum {
    VGA_BLACK           = 0x0,
    VGA_BLUE            = 0x1,
    VGA_GREEN           = 0x2,
    VGA_CYAN            = 0x3,
    VGA_RED             = 0x4,
    VGA_MAGENTA         = 0x5,
    VGA_BROWN           = 0x6,
    VGA_LIGHTGRAY       = 0x7,
    VGA_DARKGRAY        = 0x8,
    VGA_LIGHTBLUE       = 0x9,
    VGA_LIGHTGREEN      = 0xA,
    VGA_LIGHTCYAN       = 0xB,
    VGA_LIGHTRED        = 0xC,
    VGA_LIGHTMAGENTA    = 0xD,
    VGA_YELLOW          = 0xE,
    VGA_WHITE           = 0xF
} vga_colour;

typedef struct {
    uint16_t* buffer;
    uint8_t cursor_x;
    uint8_t cursor_y;
    uint8_t cell_colour;
} __attribute__((packed)) vga_screen;

static inline uint8_t vga_colour_entry(vga_colour text, vga_colour background) {
    return (background << 4) | text;
}

static inline uint16_t vga_full_entry(char entry_character, uint8_t entry_colour) {
    return ((uint16_t) entry_colour << 8) | (uint16_t) entry_character;
}

// i should prolly move this somewhere else later
static inline size_t strlen(char* data) {
    size_t count = 0;
    while (data[count]) {
        count++;
    }
    return count;
}

void int_to_hex(uint32_t n, char outp[], bool format);


void screen_init(vga_colour text, vga_colour background);
void clear_screen();

void terminal_set_colour(vga_colour text, vga_colour background);
void terminal_move_cursor(uint8_t x, uint8_t y);
void terminal_align_cursor();


void terminal_writechar(char character);
void terminal_writehex(uint32_t n, bool format);
void terminal_writestring(char* text);
void terminal_newline();
void terminal_scroll();