#include "../drivers/screen.h"

void my_function() {

}

void main() {
    screen_init(VGA_BLACK, VGA_LIGHTGREEN);
    terminal_writestring("HELLO WORLD!");
    clear_screen();
    terminal_writestring("chicken");
}