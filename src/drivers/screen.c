#include "drivers/screen.h"

static vga_screen screen;

void int_to_hex(uint32_t n, char outp[]) {
    outp[0] = '0';
    outp[1] = 'x';
    outp[10] = '\0';

    for (size_t i = 9; i > 1; i--) {
        uint32_t curr_char = n & 0xF;
        if (curr_char < 10) {
            outp[i] = '0' + curr_char;
        } else {
            outp[i] = 'A' + (curr_char - 10);
        }
        n >>= 4;
    }
}

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
    uint8_t pos_low = (uint8_t) (pos & 0xFF);
    uint8_t pos_high = (uint8_t) ((pos >> 8) & 0xFF);

    port_byte_out(IDX_PORT, 0x0F);
    port_byte_out(DATA_PORT, pos_low);

    port_byte_out(IDX_PORT, 0x0E);
    port_byte_out(DATA_PORT, pos_high);
}

void terminal_align_cursor() {
    terminal_move_cursor(screen.cursor_x, screen.cursor_y);
}

void terminal_writechar(char character) {
    if (character == '\n') {
        terminal_newline();
    } else {
        size_t addr_offset = ((screen.cursor_y * WIDTH) + screen.cursor_x);
        uint16_t entry = vga_full_entry(character, screen.cell_colour);
        screen.buffer[addr_offset] = entry;

        screen.cursor_x++;
        if (screen.cursor_x == WIDTH) {
            terminal_newline();
        } else {
            terminal_align_cursor();
        }
    }
    return;
}

void terminal_writehex(uint32_t n) {
    char buffer[10];
    int_to_hex(n, buffer);
    terminal_writestring(buffer);
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
        screen.buffer[i] = screen.buffer[i + WIDTH];
    }
    
    // clear last line if we on it
    uint16_t empty_entry = vga_full_entry(' ', screen.cell_colour);
    for (size_t i = 0; i < WIDTH; i++) {
        screen.buffer[i + ((HEIGHT - 1) * WIDTH)] = empty_entry;
    }
    screen.cursor_y--;

    terminal_align_cursor();
    return;
}