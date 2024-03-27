/*
 * This file is used to test the library
 * Creator: Hugo Coto Florez
 */

#include "bbclit.h"
#include <stdbool.h>


int main(int argc, char** arcv)
{
    enable_wide_mode();
    //hide_cursor();
    clear_screen();
    initialize_keybinds();
    MODULE_OPTIONS opt   = DEFAULT_MODULE_OPTIONS();
    struct SCREEN screen = initialize_screen();
    DIMENSION size       = fullscreen(); // no used, just to create splits
    DIMENSION size2;                     // no used
    DIMENSION size3;
    DIMENSION size4;
    DIMENSION size5;

    vsplit(size, &size2, &size3, 0.5);
    hsplit(size2, &size5, &size4, 0.25);

    BOX split1 = new_box(size3);
    BOX split2 = new_box(size4);
    BOX split3 = new_box(size5);

    draw_box(&split1, &screen);
    draw_box(&split2, &screen);
    draw_box(&split3, &screen);

    selection_box(split1, opt);
    add_entry(0, 'k', "Move up");
    add_entry(0, 'j', "Move down");

    bool i = false;
    keyboard_handler(i);

    clear_screen();
    show_cursor();

    return 0;
}
