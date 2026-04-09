#include "drivers/screen.h"

void my_function() {

}

void main() {
    screen_init(VGA_WHITE, VGA_BLACK);
    terminal_writestring("HELLO 1");
}