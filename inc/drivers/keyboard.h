#include <stdint.h>
#include <stdbool.h>

#define LARGE_CHAR_LOWER 65
#define LARGE_CHAR_UPPER 90
#define SMALL_CHAR_LOWER 97
#define SMALL_CHAR_UPPER 122

typedef struct {
    bool ctrl;
    bool alt;
    bool shift;
} modifiers;

static bool is_alpha_char(char key) {
    return (LARGE_CHAR_LOWER <= key && key <= LARGE_CHAR_UPPER) || (SMALL_CHAR_LOWER <= key && key <= SMALL_CHAR_UPPER);
}

void keyboard_init();

void modifier_key_press(uint8_t scancode, bool pressed);
void alphanum_key_press(uint8_t scancode);

void handle_key_press(uint8_t scancode);
