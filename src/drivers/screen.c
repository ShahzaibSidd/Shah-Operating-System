#include "drivers/screen.h"

static vga_screen screen;

void screen_init(vga_colour text, vga_colour background) {
    screen.cursor_x = 0;
    screen.cursor_y = 0;
    terminal_set_colour(text, background);
    screen.buffer = (uint16_t*) VGA_MEM;

    clear_screen();
}

void clear_screen() {
    screen.cursor_x = 0;
    screen.cursor_y = 0;
    
    uint16_t empty_cell = vga_full_entry(' ', screen.cell_colour);
    
    for (size_t i = 0; i < WIDTH * HEIGHT; i++) {
        screen.buffer[i] = empty_cell;
    }
    
    return;
}

void terminal_set_colour(vga_colour text, vga_colour background) {
    screen.cell_colour = vga_colour_entry(text, background);
    return;
}

void terminal_move_cursor(uint8_t x, uint8_t y) {
    uint16_t pos = (y * WIDTH) + x;
    uint8_t pos_low = (uint8_t) pos & 0xFF;
    uint8_t pos_high = (uint8_t) pos >> 8;

    port_byte_out(IDX_PORT, 0x0F);
    port_byte_out(DATA_PORT, pos_low);

    port_byte_out(IDX_PORT, 0x0E);
    port_byte_out(DATA_PORT, pos_high);
}

void terminal_align_cursor() {
    terminal_move_cursor(screen.cursor_x, screen.cursor_y);
}


void terminal_writechar(char character) {
    size_t addr_offset = ((screen.cursor_y * 80) + screen.cursor_x);

    if (character == '\n') {
        terminal_newline();
    } else {
        uint16_t entry = vga_full_entry(character, screen.cell_colour);
        screen.buffer[addr_offset] = entry;
        
        screen.cursor_x = (screen.cursor_x + 1) % WIDTH;
        if (screen.cursor_x == 0) {
            screen.cursor_y = (screen.cursor_y + 1) % HEIGHT;
        }
    }
    terminal_align_cursor();
    return;
}

void terminal_writestring(char* text) {
    size_t length = strlen(text);
    for (size_t i = 0; i < length; i++) {
        terminal_writechar(text[i]);
    }
    return;
}

void terminal_newline() {
    if (screen.cursor_y + 1 == HEIGHT) {
        terminal_scroll();
    }
    screen.cursor_x = 0;
    screen.cursor_y++;
    terminal_align_cursor();
    return;
}

void terminal_scroll() {
    // move all bytes up one row
    for (size_t i = 0; i < (HEIGHT * WIDTH) - WIDTH; i++) {
        screen.buffer[i] = screen.buffer[i + 80];
    }
    
    // clear last line if we on it
    uint16_t empty_entry = vga_full_entry(' ', screen.cell_colour);
    for (size_t i = 0; i < WIDTH; i++) {
        screen.buffer[i + (24 * 80)] = empty_entry;
    }
    screen.cursor_y--;

    terminal_align_cursor();
    return;
}