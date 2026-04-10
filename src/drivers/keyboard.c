#include "drivers/keyboard.h"
#include "drivers/screen.h"

static modifiers curr_modifiers;

void keyboard_init() {
    curr_modifiers.ctrl = false;
    curr_modifiers.alt = false;
    curr_modifiers.shift = false;
}

void modifier_key_press(uint8_t scancode, bool pressed) {
    switch (scancode) {
        case 0x1D: curr_modifiers.ctrl  = pressed; break;
        case 0x38: curr_modifiers.alt   = pressed; break;
        case 0x2A: curr_modifiers.shift = pressed; break;
    }
}

void alphanum_key_press(uint8_t scancode) {
    char pressed_key;
    switch (scancode) {
        case 0x02: pressed_key = '1'; break;
        case 0x03: pressed_key = '2'; break;
        case 0x04: pressed_key = '3'; break;
        case 0x05: pressed_key = '4'; break;
        case 0x06: pressed_key = '5'; break;
        case 0x07: pressed_key = '6'; break;
        case 0x08: pressed_key = '7'; break;
        case 0x09: pressed_key = '8'; break;
        case 0x0A: pressed_key = '9'; break;
        case 0x0B: pressed_key = '0'; break;

        case 0x10: pressed_key = 'q'; break;
        case 0x11: pressed_key = 'w'; break;
        case 0x12: pressed_key = 'e'; break;
        case 0x13: pressed_key = 'r'; break;
        case 0x14: pressed_key = 't'; break;
        case 0x15: pressed_key = 'y'; break;
        case 0x16: pressed_key = 'u'; break;
        case 0x17: pressed_key = 'i'; break;
        case 0x18: pressed_key = 'o'; break;
        case 0x19: pressed_key = 'p'; break;

        case 0x1E: pressed_key = 'a'; break;
        case 0x1F: pressed_key = 's'; break;
        case 0x20: pressed_key = 'd'; break;
        case 0x21: pressed_key = 'f'; break;
        case 0x22: pressed_key = 'g'; break;
        case 0x23: pressed_key = 'h'; break;
        case 0x24: pressed_key = 'j'; break;
        case 0x25: pressed_key = 'k'; break;
        case 0x26: pressed_key = 'l'; break;

        case 0x2C: pressed_key = 'z'; break;
        case 0x2D: pressed_key = 'x'; break;
        case 0x2E: pressed_key = 'c'; break;
        case 0x2F: pressed_key = 'v'; break;
        case 0x30: pressed_key = 'b'; break;
        case 0x31: pressed_key = 'n'; break;
        case 0x32: pressed_key = 'm'; break;

        case 0x1C: pressed_key = '\n';break;
        case 0x39: pressed_key = ' '; break;

        default:   pressed_key = ' '; break;
    }

    if (curr_modifiers.shift && is_alpha_char(pressed_key)) {
        pressed_key -= 32;
    }
    terminal_writechar(pressed_key);
}


void handle_key_press(uint8_t scancode) {
    if (scancode <= 0x80) {
        if (scancode == 0x1D || scancode == 0x38 || scancode == 0x2A) {
            modifier_key_press(scancode, true);
        } else {
            alphanum_key_press(scancode);
        }
    } else {
        scancode -= 0x80;
        if (scancode == 0x1D || scancode == 0x38 || scancode == 0x2A) {
            modifier_key_press(scancode, false);
        }
    }
}