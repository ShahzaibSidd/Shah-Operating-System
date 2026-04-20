#include "drivers/screen.h"

static vga_screen screen;

static char* init_message[18] = {
    "██╗    ██╗███████╗██╗      ██████╗ ██████╗ ███╗   ███╗███████╗",    
    "██║    ██║██╔════╝██║     ██╔════╝██╔═══██╗████╗ ████║██╔════╝",    
    "██║ █╗ ██║█████╗  ██║     ██║     ██║   ██║██╔████╔██║█████╗  ",    
    "██║███╗██║██╔══╝  ██║     ██║     ██║   ██║██║╚██╔╝██║██╔══╝  ",    
    "╚███╔███╔╝███████╗███████╗╚██████╗╚██████╔╝██║ ╚═╝ ██║███████╗",    
    " ╚══╝╚══╝ ╚══════╝╚══════╝ ╚═════╝ ╚═════╝ ╚═╝     ╚═╝╚══════╝",    
    "                    ████████╗ ██████╗                         ",    
    "                    ╚══██╔══╝██╔═══██╗                        ",    
    "                       ██║   ██║   ██║                        ",    
    "                       ██║   ██║   ██║                        ",    
    "                       ██║   ╚██████╔╝                        ",    
    "                       ╚═╝    ╚═════╝                         ",    
    "███████╗██╗  ██╗ █████╗ ██╗  ██╗     ██████╗ ███████╗██╗      ",    
    "██╔════╝██║  ██║██╔══██╗██║  ██║    ██╔═══██╗██╔════╝██║      ",    
    "███████╗███████║███████║███████║    ██║   ██║███████╗██║      ",    
    "╚════██║██╔══██║██╔══██║██╔══██║    ██║   ██║╚════██║╚═╝      ",    
    "███████║██║  ██║██║  ██║██║  ██║    ╚██████╔╝███████║██╗      ",    
    "╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝     ╚═════╝ ╚══════╝╚═╝      ",
};

static char* init_message_decoded[18] = {
    "\xDB\xDB\xBB    \xDB\xDB\xBB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBB\xDB\xDB\xBB      \xDB\xDB\xDB\xDB\xDB\xDB\xBB \xDB\xDB\xDB\xDB\xDB\xDB\xBB \xDB\xDB\xDB\xBB   \xDB\xDB\xDB\xBB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBB",
    "\xDB\xDB\xBA    \xDB\xDB\xBA\xDB\xDB\xC9\xCD\xCD\xCD\xCD\xBC\xDB\xDB\xBA     \xDB\xDB\xC9\xCD\xCD\xCD\xCD\xBC\xDB\xDB\xC9\xCD\xCD\xCD\xDB\xDB\xBB\xDB\xDB\xDB\xDB\xBB \xDB\xDB\xDB\xDB\xBA\xDB\xDB\xC9\xCD\xCD\xCD\xCD\xBC",
    "\xDB\xDB\xBA \xDB\xBB \xDB\xDB\xBA\xDB\xDB\xDB\xDB\xDB\xBB  \xDB\xDB\xBA     \xDB\xDB\xBA     \xDB\xDB\xBA   \xDB\xDB\xBA\xDB\xDB\xC9\xDB\xDB\xDB\xDB\xC9\xDB\xDB\xBA\xDB\xDB\xDB\xDB\xDB\xBB  ",
    "\xDB\xDB\xBA\xDB\xDB\xDB\xBB\xDB\xDB\xBA\xDB\xDB\xC9\xCD\xCD\xBC  \xDB\xDB\xBA     \xDB\xDB\xBA     \xDB\xDB\xBA   \xDB\xDB\xBA\xDB\xDB\xBA\xC8\xDB\xDB\xC9\xBC\xDB\xDB\xBA\xDB\xDB\xC9\xCD\xCD\xBC  ",
    "\xC8\xDB\xDB\xDB\xC9\xDB\xDB\xDB\xC9\xBC\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBB\xC8\xDB\xDB\xDB\xDB\xDB\xDB\xBB\xC8\xDB\xDB\xDB\xDB\xDB\xDB\xC9\xBC\xDB\xDB\xBA \xC8\xCD\xBC \xDB\xDB\xBA\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBB",
    " \xC8\xCD\xCD\xBC\xC8\xCD\xCD\xBC \xC8\xCD\xCD\xCD\xCD\xCD\xCD\xBC\xC8\xCD\xCD\xCD\xCD\xCD\xCD\xBC \xC8\xCD\xCD\xCD\xCD\xCD\xBC \xC8\xCD\xCD\xCD\xCD\xCD\xBC \xC8\xCD\xBC     \xC8\xCD\xBC\xC8\xCD\xCD\xCD\xCD\xCD\xCD\xBC",
    "                    \xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBB \xDB\xDB\xDB\xDB\xDB\xDB\xBB                    ",
    "                    \xC8\xCD\xCD\xDB\xDB\xC9\xCD\xCD\xBC\xDB\xDB\xC9\xCD\xCD\xCD\xDB\xDB\xBB                   ",
    "                       \xDB\xDB\xBA   \xDB\xDB\xBA   \xDB\xDB\xBA                   ",
    "                       \xDB\xDB\xBA   \xDB\xDB\xBA   \xDB\xDB\xBA                   ",
    "                       \xDB\xDB\xBA   \xC8\xDB\xDB\xDB\xDB\xDB\xDB\xC9\xBC                   ",
    "                       \xC8\xCD\xBC    \xC8\xCD\xCD\xCD\xCD\xCD\xBC                    ",
    "\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBB\xDB\xDB\xBA  \xDB\xDB\xBA \xDB\xDB\xDB\xDB\xDB\xBB \xDB\xDB\xBA  \xDB\xDB\xBA     \xDB\xDB\xDB\xDB\xDB\xDB\xBB \xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBB\xDB\xDB\xBA ",
    "\xDB\xDB\xC9\xCD\xCD\xCD\xCD\xBC\xDB\xDB\xBA  \xDB\xDB\xBA\xDB\xDB\xC9\xCD\xCD\xDB\xDB\xBB\xDB\xDB\xBA  \xDB\xDB\xBA    \xDB\xDB\xC9\xCD\xCD\xCD\xDB\xDB\xBB\xDB\xDB\xC9\xCD\xCD\xCD\xCD\xBC\xDB\xDB\xBA ",
    "\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBA\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBA\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBA    \xDB\xDB\xBA   \xDB\xDB\xBA\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBB\xDB\xDB\xBA ",
    "\xC8\xCD\xCD\xCD\xCD\xDB\xDB\xBA\xDB\xDB\xC9\xCD\xCD\xDB\xDB\xBA\xDB\xDB\xC9\xCD\xCD\xDB\xDB\xBA\xDB\xDB\xC9\xCD\xCD\xDB\xDB\xBA    \xDB\xDB\xBA   \xDB\xDB\xBA\xC8\xCD\xCD\xCD\xCD\xDB\xDB\xBA\xC8\xCD\xBC ",
    "\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBA\xDB\xDB\xBA  \xDB\xDB\xBA\xDB\xDB\xBA  \xDB\xDB\xBA\xDB\xDB\xBA  \xDB\xDB\xBA    \xC8\xDB\xDB\xDB\xDB\xDB\xDB\xC9\xBC\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBA\xDB\xDB\xBA ",
    "\xC8\xCD\xCD\xCD\xCD\xCD\xCD\xBC\xC8\xCD\xBC  \xC8\xCD\xBC\xC8\xCD\xBC  \xC8\xCD\xBC\xC8\xCD\xBC  \xC8\xCD\xBC     \xC8\xCD\xCD\xCD\xCD\xCD\xBC \xC8\xCD\xCD\xCD\xCD\xCD\xCD\xBC\xC8\xCD\xBC "
};

void int_to_hex(uint32_t n, char outp[], bool format) {
    if (format) {
        outp[0] = '0';
        outp[1] = 'x';

        for (int i = 9; i > 1; i--) {
            uint32_t curr_char = n & 0xF;
            if (curr_char < 10) {
                outp[i] = '0' + curr_char;
            } else {
                outp[i] = 'A' + (curr_char - 10);
            }
            n >>= 4;
        }
        outp[10] = '\0';

    } else {
        for (int i = 7; i >= 0; i--) {
            uint32_t curr_char = n & 0xF;
            if (curr_char < 10) {
                outp[i] = '0' + curr_char;
            } else {
                outp[i] = 'A' + (curr_char - 10);
            }
            n >>= 4;
        }   
        outp[8] = '\0';
    }
}

void screen_init(vga_colour text, vga_colour background) {
    screen.buffer = (uint16_t*) VGA_MEM;

    draw_init_anim();
    
    screen.cursor_x = 0;
    screen.cursor_y = 0;
    terminal_set_colour(text, background);

    clear_screen();
}

void clear_screen() {
    screen.cursor_x = 0;
    screen.cursor_y = 0;
    terminal_align_cursor();

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

void terminal_writechar(unsigned char character) {
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

void terminal_writehex(uint32_t n, bool format) {
    if (format) {
        char buffer[11];
        int_to_hex(n, buffer, format);
        terminal_writestring(buffer);
    } else {
        char buffer[9];
        int_to_hex(n, buffer, format);
        terminal_writestring(buffer);
    }
}

void terminal_writestring(char* text) {
    size_t length = strlen(text);
    for (size_t i = 0; i < length; i++) {
        terminal_writechar((unsigned char)text[i]);
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

void draw_welcome_msg(vga_colour text, vga_colour background, bool animate) {
    terminal_set_colour(text, background);
    clear_screen();
    screen.cursor_x = 9;
    screen.cursor_y = 3;
    for (size_t i = 0; i < 18; i++) {
        terminal_writestring(init_message_decoded[i]);
        if (animate) {
            stall(25000000);
        }
        terminal_newline();
        screen.cursor_x = 9;
    }
}

void draw_init_anim() {
    draw_welcome_msg(VGA_WHITE, VGA_BLUE, true);
    stall(0x05000000);
    draw_welcome_msg(VGA_WHITE, VGA_BROWN, false);
    stall(0x05000000);
    draw_welcome_msg(VGA_WHITE, VGA_BLUE, false);
    stall(0x05000000);
    draw_welcome_msg(VGA_WHITE, VGA_BROWN, false);
    stall(0x05000000);
    draw_welcome_msg(VGA_WHITE, VGA_BLUE, false);
    stall(0x30000000);
}